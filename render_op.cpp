//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 9:26:17 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/render_op.h"
#include "core/memory.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, render_op);

	/// default constructor
	render_op::render_op() :
		num_vertices(0),
        first_vertex(0),
		num_indices(0),
		first_index(0),
		primitive(primitive_type_invalid)
	{
		core::mem_zero(vertex_buffers, sizeof(vertex_buffers));
		core::mem_zero(index_buffers, sizeof(index_buffers));		
	}

} // end namespace
} // end namespace

