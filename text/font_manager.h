//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 30 May 2008 12:58:29 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __FONT_MANAGER_H_5E196779_E40A_4E77_8D15_03E56C562FC9_
#define __FONT_MANAGER_H_5E196779_E40A_4E77_8D15_03E56C562FC9_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/text/forward_decls.h"
#include "graphics/text/glyph_cache.h"
#include "graphics/text/string_cache.h"
#include "io/path.h"
#include "io/forward_decls.h"
#include "core/noncopyable.h"
#include <map>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace text
{

	/// The font manager is responsible for caching loaded fonts. Freetype is used as 
	/// the font engine using true type fonts and the renderer supports kerning to render
	/// super lovely fonts.
    class TYCHO_GRAPHICS_ABI font_manager : core::noncopyable
    {
	public:
		typedef string_cache::handle string_handle;
				
    public:
		/// Constructor
		font_manager(io::interface* io, graphics::renderer*);
		
		/// Destructor
		~font_manager();
		
		/// Open the font at the specified path. Will return a cached copy if has already
		/// been loaded.
		text::font_ptr open_font(const io::path& p);
								
		/// build a cached version of the passed ansi string and returns a handle that can
		/// be used to retrieve its uv coordinates
		string_handle build_string(text::font_ptr, int font_size, const char*, int num_characters);
		
		/// get UV coordinates of the string 
		const math::rectf& get_uvs(string_handle handle) const;

		/// get the string size in pixels 
		math::vector2i get_size(string_handle handle) const;
		
		/// get the texture this string is on
		texture_base_ptr get_texture(string_handle);
		
		/// release the string
		void release(string_handle);

		/// render the passed unicode string
//		bool render_string(const core::uint16*, int num_characters);
		
		/// Release all loaded fonts
		void clear();			
		   
		/// Submit all rendering needed for string caching
		void render();
		
    private:
		typedef std::map<io::path, text::font_ptr> font_map;
		
        io::interface*  m_io;
		FT_LibraryRec_* m_library;		///< FreeType library
		font_map		m_cache;		///< Cache of loaded fonts
		glyph_cache		m_glyph_cache;	///< Cache of used glyphs, this is shared across all fonts.
		string_cache    m_string_cache; ///< Cache of strings currently in use.
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __FONT_MANAGER_H_5E196779_E40A_4E77_8D15_03E56C562FC9_
