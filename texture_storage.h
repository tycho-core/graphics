//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 30 June 2008 2:11:45 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TEXTURE_STORAGE_H_AEBE18A8_71B0_455A_9FF8_3F1C4258614D_
#define __TEXTURE_STORAGE_H_AEBE18A8_71B0_455A_9FF8_3F1C4258614D_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/resource.h"
#include "graphics/format.h"
#include "image/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// device specific texture storage interface.
    class TYCHO_GRAPHICS_ABI texture_storage : virtual public graphics::resource
    {
    public:
		enum create_flags
		{
			create_invalid		= 0,
			create_immutable	= 1,
			create_dynamic		= 2,
			create_render_target = 4
		};
		
    public:
		/// create a blank texture
		/// \warning render thread only.
		virtual bool rt_create(graphics::format format, int width, int height, int num_mips, int create_flags) = 0;
		
		/// create a texture from an image object
		/// \warning render thread only.
		virtual bool rt_create(tycho::image::image_base_ptr) 
			{ return false; }

		/// update the specified part of the texture from the passed image.
		/// \warning render thread only.
		virtual bool rt_update(int x, int y, int width, int height, image::image_base* img) = 0;
		
		/// lock the texture for writing. 
		/// \warning render thread only.
		virtual bool rt_lock(int mip_level, char** out_buffer, int* out_stride) = 0;		

		/// unlock the texture. 
		/// \warning render thread only.
		virtual bool rt_unlock(int mip_level) = 0;		
		
		/// mark a section of the texture dirty
		/// \warning render thread only.
		virtual bool rt_mark_dirty(int x, int y, int width, int height) = 0;
				
    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, texture_storage)
    };

    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, texture_storage)

} // end namespace
} // end namespace

#endif // __TEXTURE_STORAGE_H_AEBE18A8_71B0_455A_9FF8_3F1C4258614D_
