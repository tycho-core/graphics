//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 3 April 2008 11:13:11 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "sysmem_vertex_buffer.h"
#include "core/debug/assert.h"
#include "core/memory.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	/// constructor
	sysmem_vertex_buffer::sysmem_vertex_buffer() :
		vertex_buffer_storage(0,0), //TODO
		m_data(0),
		m_size(0),
		m_locked(false)
	{	
	}
		
	/// destructor()
	sysmem_vertex_buffer::~sysmem_vertex_buffer()
	{
		core::safe_delete_array(m_data);
	}

	/// lock the buffer and write some data to it
	void* sysmem_vertex_buffer::lock()
	{
		TYCHO_ASSERT(m_data);
		TYCHO_ASSERT(!m_locked);
		m_locked = true;
		return m_data;
	}
	
	/// unlock the buffer
	void sysmem_vertex_buffer::unlock()
	{
		TYCHO_ASSERT(m_data);
		TYCHO_ASSERT(m_locked);
		m_locked = false;
	}
	
	/// size of the buffer
	int sysmem_vertex_buffer::size()
	{
		return m_size;
	}
	
	/// resize the buffer, may not be supported by some classes.
	bool sysmem_vertex_buffer::resize(int size, bool preserve = true)
	{
		/// \todo  would be really good here to maybe just expand the buffer if possible, need allocator support
		if(preserve)
		{
			if(!size)
			{
				core::safe_delete_array(m_data);
			}
			else
			{
				char *new_data = new char[size];
				int to_copy = std::min(m_size, size);
				if(to_copy)
					core::mem_cpy(new_data, m_data, to_copy);
				core::safe_delete_array(m_data);
				m_data = new_data;			
			}
		}
		else
		{
			core::safe_delete_array(m_data);
			if(size)
				m_data = new char[size];;		
		}
		m_size = size;
		return true;
	}


} // end namespace
} // end namespace

