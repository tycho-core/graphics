//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 2:25:23 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "parameter_buffer.h"
#include "core/memory.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho   
{
namespace graphics
{
namespace shader   
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, parameter_buffer);

	parameter_buffer::parameter_buffer(int size)
	{
		m_buffer = new core::uint8[size];
		m_size = size;
	}
	
	parameter_buffer::~parameter_buffer()
	{
		delete[] m_buffer;
	}

	/// \returns an exact copy of the buffer
	/// \warning only suitable for POD types
	parameter_buffer_ptr parameter_buffer::clone() const
	{
		parameter_buffer_ptr pb(new parameter_buffer(m_size));
		if(m_size)
		{
		    core::mem_cpy(pb->m_buffer, m_buffer, m_size);
		}
		return pb;
	}
	
} // end namespace
} // end namespace
} // end namespace
