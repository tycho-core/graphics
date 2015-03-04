//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 4 April 2008 1:00:39 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __BUFFER_STORAGE_H_4326EFBE_2934_488D_ADD6_706B90E8F701_
#define __BUFFER_STORAGE_H_4326EFBE_2934_488D_ADD6_706B90E8F701_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/resource.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

    class TYCHO_GRAPHICS_ABI buffer_storage : public graphics::resource
    {
    public:
		/// device specific implementation
		class TYCHO_GRAPHICS_ABI driver_object : public resource
		{
		public:
			virtual void* lock() = 0;
			virtual void  unlock() = 0;
			virtual int   size() const = 0;
			virtual int   get_buffer_offset() const { return 0; }
		private:
			TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, driver_object)    
		};		
		TYCHO_DECLARE_INTRUSIVE_PTR_INLINE(TYCHO_GRAPHICS_ABI, driver_object)		
    
    public:
		/// constructor
		buffer_storage(int num_bytes);
		
		/// lock the buffer and write some data to it
		void* lock();
		
		/// unlock the buffer
		void unlock();
		
		/// size of the buffer
		int size() const;
				
		/// \returns the offset of the VB in the underlying buffer, this defaults to 0 and
		///          only needs to be overwritten if buffer pooling happens in the driver.
		int get_buffer_offset() const;

		/// \returns the number of bytes the buffer uses
		int get_num_bytes() const;
		
		/// set the implementation for this object, this will be done from the render
		/// thread if multiple threads are enabled.
		void set_driver_object(driver_object_ptr);
		
		/// \returns the device specific object for this store.
		driver_object_ptr get_driver_object();

	private:		
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, buffer_storage)
    	driver_object_ptr m_driver_obj;
		int m_num_bytes; ///< size of buffer in bytes
		int m_offset;    ///< offset in bytes from the start of the buffer
	};
    
    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, buffer_storage)
	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, buffer_storage::driver_object)

} // end namespace
} // end namespace

#endif // __BUFFER_STORAGE_H_4326EFBE_2934_488D_ADD6_706B90E8F701_
