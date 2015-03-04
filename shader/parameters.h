//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 2:25:09 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PARAMETERS_H_85C19D62_E9CE_4D8F_B11F_BC9325EB3528_
#define __PARAMETERS_H_85C19D62_E9CE_4D8F_B11F_BC9325EB3528_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{
/// \todo parameters should be able to be directed straight to existing parameters of the same type
///		  should be able to create shader 'archetypes' that specify a bunch of parameters that other shaders can derive from?

	/// shader parameter types
	enum parameter_type
	{
		spt_float1,
		spt_float2,
		spt_float3,
		spt_float4,
		spt_double1,
		spt_double2,
		spt_double3,
		spt_double4,
		spt_bool1,
		spt_bool2,
		spt_bool3,
		spt_bool4,
		spt_int1,
		spt_int2,
		spt_int3,
		spt_int4,
		spt_matrix22,
		spt_matrix33,
		spt_matrix34,
		spt_matrix44,
		spt_rgba,
		spt_rgb,
		spt_texture,
	};
	
	/// \returns the size in the bytes the parameter takes up.
	int get_byte_size(parameter_type);
	
	/// \returns the alignment requirements of the parameter type
	int get_alignment(parameter_type);
	
	/// \returns string name for the parameter
	const char* get_string(parameter_type);

	/// bit flags for parameter modifiers.
	enum parameter_modifier
	{
		spm_none			= 0,
		spm_array			= (1 << 0),		///< array of the specified type. /// \todo  need size somewhere?
		spm_user_function	= (1 << 1)		///< function is bound to the parameter, this is called each time for a value.
	};
	
} // end namespace
} // end namespace
} // end namespace

#endif // __PARAMETERS_H_85C19D62_E9CE_4D8F_B11F_BC9325EB3528_
