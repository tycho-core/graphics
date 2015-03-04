//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 30 May 2008 1:00:48 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __STRING_CACHE_H_783EC652_2C82_442C_B8BF_A1213B98006C_
#define __STRING_CACHE_H_783EC652_2C82_442C_B8BF_A1213B98006C_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/shader/forward_decls.h"
#include "core/memory/allocator_2d.h"
#include "core/noncopyable.h"
#include "math/rect.h"
#include "math/quad.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace text
{

	/// Dynamic cache for strings. 
    class TYCHO_GRAPHICS_ABI string_cache : core::noncopyable
    {
    public:
		/// information for rendering a single glyph into the string cache
		struct glyph
		{
			int srcx;	 ///< source position on glyph texture
			int srcy;	 ///< source position on glyph texture
			int dstx;	 ///< destination position in string cache (in string space)
			int dsty;	 ///< destination position in string cache (in string space)
			int width;	 ///< width in pixels
			int height;  ///< height in pixels
			int texture; ///< texture to use for this glyph
		};
		
		/// opaque handle to an allocation.
		typedef void* handle;
		
		/// Maximum number of textures that can be referenced in a single render_string call.
		static const int MaxTextures = 8;
		
    public:
		/// constructor
		string_cache(graphics::renderer*, int width, int height);
		
		/// destructor
		~string_cache();
		
		/// \returns the maximum allocation width
		int get_max_width() const;
		
		/// \returns the maximum allocation height
		int get_max_height() const;
		
		/// allocate some space for strings
		handle allocate_space(int width, int height);
		
		/// free allocated space
		void free(handle);
		
		/// allocate a glyph array to write into. this is normally subsequently passed back 
		/// during a render_string operation where it takes ownership of it. In the case
		/// you do not do this you need to call free_glyph_array to dispose of it.
		glyph* allocate_glyph_array(int);
		
		/// free the glyph array
		void free_glyph_array(glyph*);
		
		/// render a string to a previously allocated slot. this object takes ownership over the glyph array
		/// \param handle		Allocation handle previously obtained from allocate_space().
		/// \param glyphs		Glyph array previously obtained from allocate_glyph_array().
		/// \param textures		Array of textures used by the glyphs
		/// \param num_textures Number of textures in the texture array
		void render_string(handle, glyph*, int num_glyphs, std::vector<graphics::texture_ptr>& textures);
		
		/// get the uv's for the passed string handle
		const math::rectf& get_string_uvs(handle) const;
		
		/// get the string size in pixels 
		math::vector2i get_string_size(handle) const;		
		
		/// \returns the texture to use for the specified handle
		graphics::texture_base_ptr get_texture(handle);		
		
		/// render all batches 
		void render_batches();
		
	private:
		/// information about a single allocation in the cache
		struct handle_
		{
			core::allocator_2d::tag*   tag;				 ///< allocator tag
			graphics::render_target_texture_ptr texture; ///< texture this string is on
			math::quad<math::vector2i> verts;			 ///< vertices in integer pixels			
			math::rectf uvs;				 ///< texture coords to render the string
		};
		handle_* make_handle(void* p) 
			{ return reinterpret_cast<handle_*>(p); }
			
		const handle_* make_handle(void* p) const
			{ return reinterpret_cast<const handle_*>(p); }
						
		struct batch
		{
			graphics::texture_ptr src_texture;
			std::vector<glyph>	  glyphs;
		};
				
		struct texture
		{
			graphics::render_target_texture_ptr m_texture;
			std::vector<batch*>	  m_batches;
		};
		
		batch* find_batch(graphics::render_target_texture_ptr dst_texture, graphics::texture_ptr src_texture);
		void batch_fill_vb(batch* b, void* vb_ptr, int vb_size);
		
    private:
		graphics::renderer*			m_renderer;
		core::allocator_2d			m_allocator;
		graphics::render_target_texture_ptr m_texture;
		std::vector<texture>		m_batches;
		shader::shader_base_ptr		m_shader;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __STRING_CACHE_H_783EC652_2C82_442C_B8BF_A1213B98006C_
