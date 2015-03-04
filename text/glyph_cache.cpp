//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 30 May 2008 1:00:55 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "glyph_cache.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"
#include "graphics/text/font.h"
#include "core/debug/assert.h"
#include "image/image_a8.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace text
{
	
	glyph_cache::glyph_cache(graphics::renderer* renderer, int width, int height) :
		m_renderer(renderer),
		m_allocator(width, height)
	{
		// create a texture to render glyphs to
		m_textures.push_back(renderer->create_dynamic_texture(graphics::format::pixel_a8, width, height));		
	}
	
	glyph_cache::~glyph_cache()
	{
	}
	
	glyph_cache::entry* glyph_cache::find(core::int64 glyph_index)
	{
		glyph_map::iterator it = m_cache.find(glyph_index);
		if(it == m_cache.end())
			return 0;
		return &it->second;
	}

	static void glyph_texture_fill_cb(texture_ptr texture, const char* buffer, 
									  int src_x, int src_y, 
									  int width, int height,
									  char* out_buffer, int out_stride)
	{
		const char* src_row = buffer + src_y * width + src_x;
		char* dst_row = out_buffer;
		for(int y = 0; y < height; ++y)
		{
			core::mem_cpy(dst_row, src_row, width);
			dst_row += out_stride;
			src_row += width;
		}
	}
	
	glyph_cache::entry* glyph_cache::add(core::int64 glyph_index, const char* buffer, int width, int height, int left_bearing, int top_bearing, const math::recti& bbox)
	{
        using namespace std::placeholders;

		// find a spot for it
		core::allocator_2d::tag* t = m_allocator.allocate(width, height);
		TYCHO_ASSERT(t);
		
		// copy glyph source data into frame allocator as it needs to live until we update in the render thread.
		char* rt_buffer = (char*)m_renderer->frame_allocate(width * height);
		core::mem_cpy(rt_buffer, buffer, width * height);				
		m_renderer->update_texture(m_textures[0], 
			t->x, t->y, 
			width, height, 
			std::bind(&glyph_texture_fill_cb, _1, rt_buffer, 0, 0, width, height, _2, _3));
			
		entry new_entry;
		new_entry.m_tag = t;
		new_entry.m_src_rect = math::recti(t->x, t->y, t->width, t->height);
		new_entry.m_bbox = bbox;
		new_entry.m_left_bearing = left_bearing;
		new_entry.m_top_bearing = top_bearing;
		new_entry.m_texture = 0;
		glyph_map::iterator it = m_cache.insert(m_cache.begin(), std::make_pair(glyph_index, new_entry));
		return &it->second;
	}
	
	void glyph_cache::clear()
	{
		// allocator doesn't delete allocated tags on clear, should change allocator to return
		// pointer to hspan as an opaque data type so we don't need to allocate ram and clear it.
		glyph_map::iterator it = m_cache.begin();
		glyph_map::iterator end = m_cache.end();
		for(; it != end; ++it)
		{
			delete it->second.m_tag;
		}			
		m_cache.clear();
		m_allocator.clear();
	}
		
	texture_ptr glyph_cache::get_texture(int i)
	{
		TYCHO_ASSERT(i == 0);
		return m_textures[i];
	}
	
	std::vector<graphics::texture_ptr>& glyph_cache::get_textures()
	{
		return m_textures;
	}

	/// \returns a font specific glyph index
	core::int64 glyph_cache::make_glyph_index(font_ptr f, int font_size, int glyph_index)
	{
		return ((core::int64)f->get_unique_id() << 32) | (font_size << 16) | glyph_index;
	}

} // end namespace
} // end namespace
} // end namespace
