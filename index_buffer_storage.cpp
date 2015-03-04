//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 22 July 2008 9:07:10 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "index_buffer_storage.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, index_buffer_storage);

    //------------------------------------------------------------------------------------

	index_buffer_storage::index_buffer_storage(int num_bytes, int create_flags) :
		buffer_storage(num_bytes),
		m_create_flags(create_flags)
	{
	}

    //------------------------------------------------------------------------------------

    int index_buffer_storage::get_create_flags() const
    {
        return m_create_flags;
    }

    //------------------------------------------------------------------------------------


} // end namespace
} // end namespace
