//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 4 April 2008 1:00:39 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "buffer_storage.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, buffer_storage)
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, buffer_storage::driver_object)

	buffer_storage::buffer_storage(int num_bytes) :
		m_num_bytes(num_bytes)
	{}

	void buffer_storage::set_driver_object(driver_object_ptr obj)
	{
		m_driver_obj = obj;
	}
	
	/// \returns the device specific object for this store.
	buffer_storage::driver_object_ptr buffer_storage::get_driver_object()
	{
		return m_driver_obj;
	}

	void* buffer_storage::lock()
	{
		TYCHO_ASSERT(m_driver_obj);
		return m_driver_obj->lock();
	}
	
	void  buffer_storage::unlock()
	{
		TYCHO_ASSERT(m_driver_obj);
		return m_driver_obj->unlock();
	}

	int buffer_storage::size() const
	{
		TYCHO_ASSERT(m_driver_obj);
		return m_num_bytes;
	}

	int buffer_storage::get_buffer_offset() const
	{
		TYCHO_ASSERT(m_driver_obj);
		return m_driver_obj->get_buffer_offset();
	}

} // end namespace
} // end namespace
