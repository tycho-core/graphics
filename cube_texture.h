//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 7:01:57 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __CUBE_TEXTURE_H_F1942747_0101_475F_8DAC_B7D0BF951D6F_
#define __CUBE_TEXTURE_H_F1942747_0101_475F_8DAC_B7D0BF951D6F_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/resource.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

    class TYCHO_GRAPHICS_ABI cube_texture : public graphics::resource
    {
    public:
    
    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, cube_texture)
    };

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, cube_texture)
	
} // end namespace
} // end namespace

#endif // __CUBE_TEXTURE_H_F1942747_0101_475F_8DAC_B7D0BF951D6F_
