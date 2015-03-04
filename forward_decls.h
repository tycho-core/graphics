//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 7:12:27 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __FORWARD_DECLS_H_A04C0BFF_EE6E_4122_BBB4_7401ECD965D2_
#define __FORWARD_DECLS_H_A04C0BFF_EE6E_4122_BBB4_7401ECD965D2_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "core/intrusive_ptr.h"
#include "core/shared_ptr.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	struct viewport;

	TYCHO_DECLARE_SHARED_PTR(TYCHO_GRAPHICS_ABI, system_shader_factory)
	TYCHO_DECLARE_SHARED_PTR(TYCHO_GRAPHICS_ABI, render_interface)
	TYCHO_DECLARE_SHARED_PTR(TYCHO_GRAPHICS_ABI, renderer)
	
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, texture_base)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, texture)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, vertex_buffer)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, index_buffer)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, texture_storage)	
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, vertex_buffer_storage)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, index_buffer_storage)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, cube_texture)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, volume_texture)
    TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, camera)

	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, resource)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, adapter)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, render_target)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, render_target_storage)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, render_target_texture_storage)	
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, render_target_texture)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, window)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, window_storage)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, render_op)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, vertex_format)
	TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_GRAPHICS_ABI, mesh)
	
} // end namespace
} // end namespace

#endif // __FORWARD_DECLS_H_A04C0BFF_EE6E_4122_BBB4_7401ECD965D2_
