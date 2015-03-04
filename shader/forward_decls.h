//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 2:53:52 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __FORWARD_DECLS_H_19B139EB_1324_4DC4_81AA_38D9A8CB932B_
#define __FORWARD_DECLS_H_19B139EB_1324_4DC4_81AA_38D9A8CB932B_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "core/intrusive_ptr.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{

	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, gamecube_shader);
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, shader_base);
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, parameter_buffer);
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, parameter_buffer_layout);
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, parameter_object_lookup_table);
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, parameter_bindings);
	
	class parameter_buffer_view;
		
} // end namespace
} // end namespace
} // end namespace

#endif // __FORWARD_DECLS_H_19B139EB_1324_4DC4_81AA_38D9A8CB932B_
