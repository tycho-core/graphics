//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 30 May 2008 12:58:30 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/text/font_manager.h"
#include "graphics/text/font.h"
#include "core/debug/assert.h"
#include "io/stream.h"
#include "io/interface.h"
#include "core/memory.h"
#include "core/globals.h"
#include "core/string.h"

// freetype headers
#include "ft2build.h"
#include FT_FREETYPE_H 
#include FT_MODULE_H
#include FT_GLYPH_H

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

extern const FT_Module_Class tt_driver_class;

namespace tycho
{
namespace graphics
{
namespace text
{
	namespace detail
	{
		
		///////////////////////////////////////////////////////////////////////////////////////
		// Freetype allocation functions
		///////////////////////////////////////////////////////////////////////////////////////

		void* ft_alloc(FT_Memory /*memory*/, long size)		
		{		
			return core::malloc( size );
		}		
			
		void* ft_realloc(FT_Memory /*memory*/, long /*cur_size*/, long new_size, void* block)
		{
			return core::realloc(block, new_size);
		}		
		
		void ft_free(FT_Memory /*memory*/, void* block)
		{
			core::free(block);
		}		

		///////////////////////////////////////////////////////////////////////////////////////
		// Misc
		///////////////////////////////////////////////////////////////////////////////////////

		// information for a single glyph during rendering
		struct glyph
		{
			FT_UInt	  index;	// glyph index
			FT_Vector pos;		// glyph origin on the baseline
			FT_Glyph  bitmap;	// glyph image
			glyph_cache::entry* cached_entry;
		};
		typedef std::vector<glyph> glyph_list;

		FT_BBox calc_string_bbox(const glyph_list& glyphs)
		{
			FT_BBox bbox;
			
			// initialize string bbox to "empty" values
			bbox.xMin = bbox.yMin = 32000; 
			bbox.xMax = bbox.yMax = -32000; 
			
			// for each glyph image, compute its bounding box
			// translate it, and grow the string bbox
			for(size_t n = 0; n < glyphs.size(); n++) 
			{ 
				FT_BBox glyph_bbox; 
				math::recti tmp_bbox = glyphs[n].cached_entry->get_bbox();
				glyph_bbox.xMin = tmp_bbox.get_minx();
				glyph_bbox.xMax = tmp_bbox.get_maxx();
				glyph_bbox.yMin = tmp_bbox.get_miny();
				glyph_bbox.yMax = tmp_bbox.get_maxy();
				glyph_bbox.xMin += glyphs[n].pos.x; 
				glyph_bbox.xMax += glyphs[n].pos.x; 
				glyph_bbox.yMin += glyphs[n].pos.y; 
				glyph_bbox.yMax += glyphs[n].pos.y; 
				
				if(glyph_bbox.xMin < bbox.xMin) 
					bbox.xMin = glyph_bbox.xMin; 
				if(glyph_bbox.yMin < bbox.yMin) 
					bbox.yMin = glyph_bbox.yMin; 
				if(glyph_bbox.xMax > bbox.xMax) 
					bbox.xMax = glyph_bbox.xMax; 
				if(glyph_bbox.yMax > bbox.yMax) 
					bbox.yMax = glyph_bbox.yMax; 
			} 
			
			// check that we really grew the string bbox
			if(bbox.xMin > bbox.xMax) 
			{ 
				bbox.xMin = 0; 
				bbox.yMin = 0; 
				bbox.xMax = 0; 
				bbox.yMax = 0; 
			} 			
			return bbox;
		}
		
	} // end namespace

	font_manager::font_manager(io::interface* io, graphics::renderer* r) :
        m_io(io),
		m_library(0),
		m_glyph_cache(r, 512, 512),
		m_string_cache(r, 1024, 1024)
	{
		// reroute freetype's memory handler to ours
		FT_Memory memory = (FT_Memory)core::malloc(sizeof(*memory));
		if(memory)
		{
			memory->user    = 0;
			memory->alloc   = detail::ft_alloc;
			memory->realloc = detail::ft_realloc;
			memory->free    = detail::ft_free;
		}
#ifdef TYCHO_ASSERT_ENABLED
		int error =
#endif
            FT_New_Library(memory, &m_library);
		TYCHO_ASSERT(!error);
 		FT_Add_Default_Modules(m_library);
	}
	
	font_manager::~font_manager()
	{
		// shutdown the freetype library
		m_cache.clear();
		m_glyph_cache.clear();
		if(m_library)
		{
			FT_Done_FreeType(m_library);
			m_library = 0;
		}
	}
	
	text::font_ptr font_manager::open_font(const io::path& p)
	{
		// check to see if we have this already cached
		font_map::iterator it = m_cache.find(p);
		if(it != m_cache.end())
			return it->second;
			
		// the source date is read entirely into memory as it continuously reference during string 
		// formatting. Fonts may be quite large so we may want either compression or some stripped
		// down version of the data.
		io::stream_ptr str = m_io->open_stream(p, io::open_flag_read);
		if(!str)
			return text::font_ptr();
		
		font_ptr new_font(new text::font());
		if(!str->read_all(new_font->m_data))
			return text::font_ptr();					
		
		FT_Open_Args args;
		args.flags = FT_OPEN_MEMORY | FT_OPEN_DRIVER;
		args.pathname = (char*)p.string();
		args.driver = FT_Get_Module(m_library, "truetype");
		args.memory_base = (FT_Byte*)&new_font->m_data[0];
		args.memory_size = new_font->m_data.size();		
		
		FT_Face face;
		int error = FT_Open_Face(m_library, &args, 0, &face);
		if(error)
			return text::font_ptr();
		new_font->m_face = face;
		
		FT_Set_Char_Size(face, 0, 48 * 64, 76, 76);
		
		// add to cache
		m_cache.insert(std::make_pair(p, new_font));
		return new_font;
	}
				

	font_manager::string_handle font_manager::build_string(text::font_ptr font, int font_size, const char* str, int num_characters)
	{
		TYCHO_ASSERT(font_size > 0);
		bool kerning = true;
		FT_Face face = font->m_face;
		int error = FT_Set_Pixel_Sizes(face, 0, font_size);
		if(error)
			return 0;		
			
		if(num_characters == -1)
			num_characters = core::strlen(str);
		
		detail::glyph_list glyphs;
		FT_GlyphSlot slot = face->glyph;
		FT_UInt previous_glyph = 0;
		FT_Bool use_kerning = kerning;
		int pen_x = 0, pen_y = 0;		
		use_kerning = kerning && FT_HAS_KERNING(face);
		for(int i = 0; i < num_characters; ++i)
		{
			const char cur_char = str[i];
			detail::glyph cur_glyph;
			cur_glyph.index = FT_Get_Char_Index(face, cur_char);
			FT_Load_Glyph(face, cur_glyph.index, FT_LOAD_DEFAULT);
			if(cur_char != ' ')
			{
				cur_glyph.cached_entry = m_glyph_cache.find(glyph_cache::make_glyph_index(font, font_size, cur_glyph.index));
				if(!cur_glyph.cached_entry)
				{
					// glyph not cached so render it and add.
					int error = FT_Load_Char(face, str[i], FT_LOAD_RENDER);
					if(error)
						return 0;
					FT_BBox ft_glyph_bbox; 
					FT_Glyph image;
					FT_Get_Glyph(face->glyph, &image);
					FT_BitmapGlyph bitmap = (FT_BitmapGlyph)image;
					FT_Glyph_Get_CBox(image, ft_glyph_bbox_pixels, &ft_glyph_bbox); 										
					math::recti glyph_bbox(ft_glyph_bbox.xMin, ft_glyph_bbox.yMin, ft_glyph_bbox.xMax - ft_glyph_bbox.xMin, ft_glyph_bbox.yMax - ft_glyph_bbox.yMin);
					cur_glyph.cached_entry = m_glyph_cache.add(glyph_cache::make_glyph_index(font, font_size, cur_glyph.index), 
						(const char*)slot->bitmap.buffer, 
						bitmap->bitmap.width,
						bitmap->bitmap.rows,
						bitmap->left,
						bitmap->top,
						glyph_bbox);
					FT_Done_Glyph(image);
				}						
			}
			
			// retrieve kerning distance and move pen
			if(use_kerning && previous_glyph && cur_glyph.index)
			{
				FT_Vector delta;
				FT_Get_Kerning(face, previous_glyph, cur_glyph.index, FT_KERNING_DEFAULT, &delta);
				pen_x += delta.x >> 6; 
			}
			cur_glyph.pos.x = pen_x;
			cur_glyph.pos.y = pen_y;			
			pen_x += slot->advance.x >> 6;	
			previous_glyph = cur_glyph.index;
			if(cur_char != ' ')			
				glyphs.push_back(cur_glyph);			
		}
		FT_BBox string_bounds = detail::calc_string_bbox(glyphs);
		int width = string_bounds.xMax - string_bounds.xMin;
		int height = string_bounds.yMax - string_bounds.yMin;
		TYCHO_ASSERT(width);
		TYCHO_ASSERT(height);
		
		// allocate enough space in the string cache to render the string
		const int padding = 2; // couple of pixels padding to avoid filtering artifacts
		string_cache::handle handle = m_string_cache.allocate_space(width + padding, height);
		if(!handle)
		{
            TYCHO_ASSERT_MSG(handle, "String is too large to be cached, increase the cache size or shorten the string");
            return nullptr;
		}
		
		// calculate the maximum top bearing for the string
		int max_top_bearing = -1;
		for(size_t i = 0; i < glyphs.size(); ++i)
		{
			const detail::glyph& g = glyphs[i];
			int tb = g.cached_entry->get_top_bearing();
			if(tb > max_top_bearing)
				max_top_bearing = tb;
		}		
		TYCHO_ASSERT(max_top_bearing != -1);		
				
		// get a glyph array to render this string
		string_cache::glyph* render_glyphs = m_string_cache.allocate_glyph_array(glyphs.size());
		TYCHO_ASSERT(render_glyphs);
		string_cache::glyph* cur_glyph = render_glyphs;
		for(size_t i = 0; i < glyphs.size(); ++i, ++cur_glyph)
		{
			const detail::glyph& g = glyphs[i];
			const math::recti& src_rect = g.cached_entry->get_texture_rect();			

			cur_glyph->dstx = g.pos.x + ((i == 0) ? 0 : g.cached_entry->get_left_bearing());
			cur_glyph->dsty = g.pos.y - g.cached_entry->get_top_bearing() + max_top_bearing;
			cur_glyph->srcx = src_rect.get_top_left().x();
			cur_glyph->srcy = src_rect.get_top_left().y();
			cur_glyph->width = src_rect.get_width();
			cur_glyph->height = src_rect.get_height();			
			cur_glyph->texture = g.cached_entry->get_texture();
		}		
		m_string_cache.render_string(handle, render_glyphs, glyphs.size(), m_glyph_cache.get_textures());				
				
		// and return a handle to this string to the caller		
		return handle;
	} 
		
	const math::rectf& font_manager::get_uvs(string_handle handle) const
	{
		TYCHO_ASSERT(handle);
		return m_string_cache.get_string_uvs(handle);
	}

	
	math::vector2i font_manager::get_size(string_handle handle) const
	{
		TYCHO_ASSERT(handle);
		return m_string_cache.get_string_size(handle);
	}
	
	texture_base_ptr font_manager::get_texture(string_handle handle)
	{
		TYCHO_ASSERT(handle);
		return m_string_cache.get_texture(handle);
	}
	
	void font_manager::release(string_handle handle)
	{
		TYCHO_ASSERT(handle);
		m_string_cache.free(handle);
	}
		
	void font_manager::clear()
	{
		m_cache.clear();
	}

	void font_manager::render()
	{
		m_string_cache.render_batches();
	}

} // end namespace
} // end namespace
} // end namespace
