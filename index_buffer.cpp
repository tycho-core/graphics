//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:59:01 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "index_buffer.h"
#include "graphics/index_buffer_storage.h"
#include "core/debug/assert.h"
#include "core/memory.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, index_buffer);

	/// constructor
	index_buffer::index_buffer(format f, int num_indices) : 
		m_format(f),
		m_store(0),
		m_num_indices(num_indices)
	{	
	}

	bool index_buffer::create(format f, index_buffer_storage_ptr s)
	{
		TYCHO_ASSERT(s);
		m_format = f;
		m_store = s;
		m_num_indices = m_store->size() / get_num_bytes(m_format);
		return true;
	}

	/// destructor
	index_buffer::~index_buffer()
	{
	}

	/// format of the index buffer
	index_buffer::format index_buffer::get_format() const
	{ 
		return m_format; 
	}

	/// lock the buffer
	void* index_buffer::lock()
	{
		TYCHO_ASSERT(m_store);
		return m_store->lock();
	}
	
	/// unlock the buffer
	void index_buffer::unlock()
	{
		TYCHO_ASSERT(m_store);
		return m_store->unlock();	
	}
	
	/// get the size of the index buffer in bytes
	int index_buffer::get_bytes_used() const
	{
		TYCHO_ASSERT(m_store);
		return m_store->size();
	}

    /// \returns the number of bytes in the passed format
    int index_buffer::get_num_bytes(format f)
    {
		switch(f)
		{
			case f_int8 : return 1;
			case f_int16 : return 2;
			case f_int32 : return 4;
                
            default: break;
		}
		TYCHO_ASSERT(!"invalid format");
		return -1;
    }
    
	/// \returns the number of indices that can fit in the buffer
	int index_buffer::get_num_indices() const
	{
		return m_num_indices;
    }
    
    
	/// \returns the offset of this index buffer in the underlying buffer
	int index_buffer::get_buffer_offset() const
	{
		return m_store->get_buffer_offset();
	}
	
	/// \returns underlying storage
	index_buffer_storage_ptr index_buffer::get_storage()
	{
		return m_store;
	}

	void index_buffer::set_storage(index_buffer_storage_ptr s)
	{
		m_store = s;
	}
    
	/// copy the input indices to the output possibly converting between types
	bool index_buffer::write(format src_format, const void* src_ptr, format dst_format, void* dst_ptr, int num_indices)
	{
		if(src_format == dst_format)
		{
			core::mem_cpy(dst_ptr, src_ptr, num_indices * get_num_bytes(src_format));
			return true;
		}
		else
		{
			// conversion needed, only six possibilities so just enumerate them all
			if(src_format == f_int8)
			{
			
			}
			else if(src_format == f_int16)
			{
			
			}
			else
			{
				TYCHO_ASSERT(src_format == f_int32);
			}
			
			TYCHO_NOT_IMPLEMENTED
			return false;
		}
	}
	
	/// \returns the size in bytes for N indices of the specified type
	int index_buffer::get_num_bytes(format f, int n)
	{
		return get_num_bytes(f) * n;
	}

} // end namespace
} // end namespace
