//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 18 May 2008 5:01:36 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TEXTURE_UTILS_H_CE20F32D_F655_4E1F_93E3_9E805C8BABB2_
#define __TEXTURE_UTILS_H_CE20F32D_F655_4E1F_93E3_9E805C8BABB2_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "io/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	TYCHO_GRAPHICS_ABI graphics::texture_ptr create_texture_from_memory_png(graphics::renderer_ptr renderer, void* data, int len);
	TYCHO_GRAPHICS_ABI graphics::texture_ptr create_texture_from_file_png(io::interface*, graphics::renderer_ptr renderer, const char* path);
	
} // end namespace
} // end namespace

#endif // __TEXTURE_UTILS_H_CE20F32D_F655_4E1F_93Ex3_9E805C8BABB2_
