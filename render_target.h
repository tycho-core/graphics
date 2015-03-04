//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 8:04:41 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDER_TARGET_H_526C001A_3FA7_4351_AF37_A39886402398_
#define __RENDER_TARGET_H_526C001A_3FA7_4351_AF37_A39886402398_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/resource.h"
#include "graphics/texture_storage.h"
#include "graphics/render_target_storage.h"
#include "core/colour/rgba.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// render target interface. in practice this is either a window or a texture.
	class TYCHO_GRAPHICS_ABI render_target : public virtual resource
	{
	public:			
		enum clear_flags
		{
			clear_none    = 0,
			clear_colour  = 1,
			clear_zbuffer = 2,
			clear_stencil = 4
		};
		
		/// virtual destructor
		virtual ~render_target() {};
				
		/// \returns the width of the render target
		virtual int get_width() const = 0;
		
		/// \returns the height of the render target
		virtual int get_height() const = 0;	        

		virtual graphics::texture_storage_ptr get_texture_storage() 
			{ return 0; }
			
		virtual graphics::render_target_storage_ptr get_render_target_storage() = 0;
			
	private:
		friend class renderer;				
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, render_target)
		core::rgba m_clear_clr;
		int		   m_clear_flags;
	};
	
	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, render_target)

} // end namespace
} // end namespace

#endif // __RENDER_TARGET_H_526C001A_3FA7_4351_AF37_A39886402398_
