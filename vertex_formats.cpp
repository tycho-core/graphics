//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 20 May 2008 1:38:42 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "vertex_formats.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
#define TYCHO_DEFINE_VERTEX_FORMAT(_fmt) \
	TYCHO_GRAPHICS_ABI vertex_format vertex_formats::_fmt (_fmt##_components, _fmt##_semantics);

	static vertex_format::component_decl pos3f32_nml3f32_uv2f32_components[] = { 
		{ vertex_format::ct_float, 3 }, 
		{ vertex_format::ct_float, 3 },
		{ vertex_format::ct_float, 2 },
		{ vertex_format::ct_invalid } 
		};
		
	static vertex_format::semantic_decl  pos3f32_nml3f32_uv2f32_semantics[] = { 
		{ vertex_format::s_position, 0, 0, vertex_format::c_xyz }, 
		{ vertex_format::s_normal,   0, 1, vertex_format::c_xyz }, 
		{ vertex_format::s_texcoord, 0, 2, vertex_format::c_xy  },
		{ vertex_format::s_invalid } 
		};		

	TYCHO_DEFINE_VERTEX_FORMAT(pos3f32_nml3f32_uv2f32);

	vertex_format::component_decl pos3f32_nml3f32_components[] = { 
		{ vertex_format::ct_float, 3 }, 
		{ vertex_format::ct_float, 3 },
		{ vertex_format::ct_invalid } 
		};
		
	vertex_format::semantic_decl  pos3f32_nml3f32_semantics[] = { 
		{ vertex_format::s_position, 0, 0, vertex_format::c_xyz }, 
		{ vertex_format::s_normal,   0, 1, vertex_format::c_xyz }, 
		{ vertex_format::s_invalid } 
		};		
	TYCHO_DEFINE_VERTEX_FORMAT(pos3f32_nml3f32);

} // end namespace
} // end namespace
