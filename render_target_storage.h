//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 6 July 2008 11:26:45 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDER_TARGET_STORAGE_H_AF94484C_0674_4848_AE0B_5F32C6889CC5_
#define __RENDER_TARGET_STORAGE_H_AF94484C_0674_4848_AE0B_5F32C6889CC5_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/resource.h"
#include "graphics/format.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

    class TYCHO_GRAPHICS_ABI render_target_storage : virtual public graphics::resource
    {
    public:
		/// create the render target
//		virtual bool rt_create(graphics::format format, int width, int height, int num_mips) = 0;
    
		/// called before rendering starts
		virtual void rt_begin_scene() = 0;
		
		/// called after rendering has completed
		virtual void rt_end_scene() = 0;
		
		/// clear the render target
		virtual void rt_clear() = 0;
		
		/// present the target to a output device, only valid for window render targets.
		virtual void rt_present() = 0;

    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, render_target_storage)
    };

    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, render_target_storage)

} // end namespace
} // end namespace

#endif // __RENDER_TARGET_STORAGE_H_AF94484C_0674_4848_AE0B_5F32C6889CC5_
