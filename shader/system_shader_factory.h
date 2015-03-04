//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 3:05:15 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SYSTEM_SHADER_FACTORY_H_5AD6C733_2632_4F58_856F_8E52E94DE048_
#define __SYSTEM_SHADER_FACTORY_H_5AD6C733_2632_4F58_856F_8E52E94DE048_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/shader/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{
	/// System shaders are very basic shaders that each platform exposes that are
	/// guaranteed to be available. These are used for debug rendering and testing
	/// without the need for one of the fully fledged shader factories and shouldn't
	/// be implements in terms of those as its useful to allow the use of these very 
	/// early on in the system startup
    class TYCHO_GRAPHICS_ABI system_shader_factory
    {
    public:
		enum shader_type
		{
			st_screen_space, 
			st_3d_clr
		};
		
    public:
		virtual shader_base_ptr create(shader_type) = 0;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __SYSTEM_SHADER_FACTORY_H_5AD6C733_2632_4F58_856F_8E52E94DE048_
