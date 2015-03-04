//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 16 April 2008 11:57:43 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __UTILITY_H_94BF9928_99CE_41A3_A241_CC510A374A7C_
#define __UTILITY_H_94BF9928_99CE_41A3_A241_CC510A374A7C_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/render_op.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	
	/// \returns the number of primitives for the number of vertices.
	TYCHO_GRAPHICS_ABI int get_num_primitives(int num_verts, primitive_type);

} // end namespace
} // end namespace

#endif // __UTILITY_H_94BF9928_99CE_41A3_A241_CC510A374A7C_