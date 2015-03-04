//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 6 July 2008 11:03:41 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TEXTURE_BASE_H_36E0F477_9162_4DB6_A065_824C596B0240_
#define __TEXTURE_BASE_H_36E0F477_9162_4DB6_A065_824C596B0240_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/resource.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// base class for all textures
    class TYCHO_GRAPHICS_ABI texture_base : public virtual graphics::resource
    {
    public:		
		/// destructor
		virtual ~texture_base() {}

		/// \returns the underlying storage for this texture
		virtual graphics::texture_storage_ptr get_texture_storage() = 0;
						
		/// \returns the width of the texture
		virtual int get_width() const = 0;
		
		/// \returns the height of the texture
		virtual int get_height() const = 0;
		
		/// \returns the number of mip levels in the texture
		virtual int get_num_mips() const = 0;
    
    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, texture_base)        
    };

    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, texture_base)

} // end namespace
} // end namespace

#endif // __TEXTURE_BASE_H_36E0F477_9162_4DB6_A065_824C596B0240_
