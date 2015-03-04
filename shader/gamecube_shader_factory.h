//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 10 April 2008 7:41:42 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __GAMECUBE_SHADER_FACTORY_H_F0CF1B32_CEE5_46A7_BD6A_CAD1AA0738F2_
#define __GAMECUBE_SHADER_FACTORY_H_F0CF1B32_CEE5_46A7_BD6A_CAD1AA0738F2_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/shader/gamecube_shader.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{

	/// This is responsible for creating new gamecube shaders. Internally it will
	/// cache generated shaders and return one of those if it is identical to a 
	/// requested one.
    class TYCHO_GRAPHICS_ABI gamecube_shader_factory
    {
    public:
		/// create a new shader from the description in the passed null terminated string.
		gamecube_shader_ptr create(const char *str);		
		
    private:
    
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __GAMECUBE_SHADER_FACTORY_H_F0CF1B32_CEE5_46A7_BD6A_CAD1AA0738F2_
