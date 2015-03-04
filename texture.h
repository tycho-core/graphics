//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:58:28 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TEXTURE_H_049CCE84_B448_4175_8AC6_DE549EBAE077_
#define __TEXTURE_H_049CCE84_B448_4175_8AC6_DE549EBAE077_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/resource.h"
#include "graphics/forward_decls.h"
#include "graphics/format.h"
#include "graphics/texture_base.h"
#include "image/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

    class TYCHO_GRAPHICS_ABI texture : public texture_base
    {
    public:
		/// constructor
		texture(graphics::renderer*, int width, int height, int num_mips);
		
		/// \returns the underlying storage for this texture
		graphics::texture_storage_ptr get_texture_storage();
			
		/// set the underlying storage for this texture
		void set_storage(graphics::texture_storage_ptr store) ;
			
		/// \returns the width of the texture
		int get_width() const;
		
		/// \returns the height of the texture
		int get_height() const;
		
		/// \returns the number of mip levels in the texture
		int get_num_mips() const;
								
    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, texture)    
		graphics::texture_storage_ptr m_storage;
		int	m_width;
		int m_height;
		int m_num_mips;
    };
    
    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, texture)

} // end namespace
} // end namespace

#endif // __TEXTURE_H_049CCE84_B448_4175_8AC6_DE549EBAE077_
