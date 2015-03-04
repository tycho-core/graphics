//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 3 April 2008 11:17:12 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "vertex_buffer_storage.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, vertex_buffer_storage);

    //------------------------------------------------------------------------------------

    vertex_buffer_storage::vertex_buffer_storage(int num_bytes, int create_flags) :
		buffer_storage(num_bytes),
		m_create_flags(create_flags)
	{}

    //------------------------------------------------------------------------------------

    int vertex_buffer_storage::get_create_flags() const
    {
        return m_create_flags;
    }

    //------------------------------------------------------------------------------------

} // end namespace
} // end namespace
