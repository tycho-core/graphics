//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 30 May 2008 1:00:55 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __GLYPH_CACHE_H_47807B16_2F4D_4DB2_86AC_B5DA3AE3D6A2_
#define __GLYPH_CACHE_H_47807B16_2F4D_4DB2_86AC_B5DA3AE3D6A2_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/text/forward_decls.h"
#include "core/memory/allocator_2d.h"
#include "math/rect.h"
#include <map>
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

	/// simple glyph cache
    class TYCHO_GRAPHICS_ABI glyph_cache
    {
	public:
		struct entry
		{
		public:
			entry() : m_tag(0) {}
			
			const math::recti& get_texture_rect() const
				{ return m_src_rect; }
				
			const math::recti& get_bbox() const
				{ return m_bbox; }
		
			int get_texture() const
				{ return m_texture; }
				
			int get_left_bearing() const 
				{ return m_left_bearing; }

			int get_top_bearing() const 
				{ return m_top_bearing; }
				 
		private:
			friend class glyph_cache;
			core::allocator_2d::tag* m_tag;
			int		    m_texture; ///< texture this glyph uses
			math::recti m_src_rect;
			math::recti m_bbox;
			int		    m_left_bearing; // in integer pixels
			int			m_top_bearing; // in integer pixels
		};
				
    public:
		/// constructor
		glyph_cache(graphics::renderer*, int width, int height);
		
		/// destructor
		~glyph_cache();
		
		/// retrieve a glyph from its past glyph index (not its character code).
		entry* find(core::int64 glyph_index);
		
		/// add a glyph to the cache
		entry* add(core::int64 glyph_index, const char* buffer, int width, int height, int left_bearing, int top_bearing, const math::recti& bbox);		
		
		/// get the i'th texture. 
		texture_ptr get_texture(int i);
		
		/// get cache textures array 
		std::vector<graphics::texture_ptr>& get_textures();
								
		/// clear the cache
		void clear();
		
		/// \returns a font specific glyph index
		static core::int64 make_glyph_index(font_ptr f, int font_size, int glyph_index);
		
    private:		
		typedef std::map<core::int64, entry> glyph_map;
		graphics::renderer*		m_renderer;
		core::allocator_2d		m_allocator;
		glyph_map				m_cache;
		std::vector<graphics::texture_ptr>	m_textures;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __GLYPH_CACHE_H_47807B16_2F4D_4DB2_86AC_B5DA3AE3D6A2_
