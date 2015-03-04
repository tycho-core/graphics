//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 2:25:09 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "parameters.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace shader
{

	/// \returns the size in the bytes the parameter takes up.
	int get_byte_size(parameter_type t)
	{
		switch(t)
		{
			case spt_float1		: return 4;
			case spt_float2		: return 8;
			case spt_float3		: return 12;
			case spt_float4		: return 16;
			case spt_double1	: return 8;
			case spt_double2	: return 16;
			case spt_double3	: return 24;
			case spt_double4	: return 32;
			case spt_bool1		: return 4;
			case spt_bool2		: return 8;
			case spt_bool3		: return 12;
			case spt_bool4		: return 16;
			case spt_int1		: return 4;
			case spt_int2		: return 8;
			case spt_int3		: return 12;
			case spt_int4		: return 16;
			case spt_matrix22	: return 16;
			case spt_matrix33	: return 36;
			case spt_matrix34	: return 48;
			case spt_matrix44	: return 64;
			case spt_rgba		: return 4;
			case spt_rgb		: return 4;
			case spt_texture	: return 4;
			
			default:
				TYCHO_ASSERT(!"invalid shader parameter type");
		}
		
		return -1;
	}
	
	/// \returns the alignment requirements of the parameter type
	int get_alignment(parameter_type t)
	{
		switch(t)
		{
			case spt_double1	: 
			case spt_double2	: 
			case spt_double3	: 
			case spt_double4	: return 8;
			case spt_float1		: 
			case spt_float2		: 
			case spt_float3		: 
			case spt_float4		: 
			case spt_bool1		: 
			case spt_bool2		: 
			case spt_bool3		: 
			case spt_bool4		:
			case spt_int1		: 
			case spt_int2		: 
			case spt_int3		: 
			case spt_int4		: 
			case spt_matrix22	: 
			case spt_matrix33	: 
			case spt_matrix34	: 
			case spt_matrix44	: 
			case spt_rgba		: 
			case spt_rgb		: 
			case spt_texture	: return 4;
			
			default:
				TYCHO_ASSERT(!"invalid shader parameter type");
		}
		
		return -1;	
	}
	
	/// \returns string name for the parameter
	const char* get_string(parameter_type)
	{
		return "<IMPLEMENT ME!>";
	}

} // end namespace
} // end namespace
} // end namespace
