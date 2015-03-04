//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:59:01 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __INDEX_BUFFER_H_37CA509A_1802_43CC_848B_EBF11A5EB338_
#define __INDEX_BUFFER_H_37CA509A_1802_43CC_848B_EBF11A5EB338_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/resource.h"
#include "graphics/scoped_buffer_lock.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{

	/// interface to GPU vertex buffer. this types (through the vertex_format desc) the underlying
	/// raw storage buffer.
    class TYCHO_GRAPHICS_ABI index_buffer : virtual public graphics::resource
    {
    public:
		/// flags to help guide the driver when allocating it
		enum create_flags
		{
			cf_static,
			cf_dynamic ///< hint to say the buffer will be updated frequently. default is static.
		};
        
        /// format of the indices
        enum format
        {
			f_invalid,
			f_int8,
			f_int16,
			f_int32        
        };
                
    public:
		/// constructor
		index_buffer(format, int num_indices);

		/// constructor, takes ownership of the passed pointers
		bool create(format, index_buffer_storage_ptr);
		
		/// destructor
		~index_buffer();
    
		/// lock the buffer
		void* lock();
		
		/// unlock the buffer
		void unlock();
		
		/// attempt to resize the buffer
		bool resize(int num_verts, bool preserve = true);
		
		/// format of the vertex buffer
		format get_format() const;
			
		/// get the size of the vertex buffer in bytes
		int get_bytes_used() const;

		/// \returns the number of indices that can fit in the buffer
		int get_num_indices() const;

		/// \returns the offset of this index buffer in the underlying buffer
		int get_buffer_offset() const;
		
		/// \returns underlying storage
		index_buffer_storage_ptr get_storage();
		
		/// set the underlying storage
		void set_storage(index_buffer_storage_ptr);
		
		/// copy the input indices to the output possibly converting between types
		static bool write(format src_format, const void* src_ptr, format dst_format, void* dst_ptr, int num_indices);
		
		/// \returns the size in bytes to hold a single index of the passed format
		static int get_num_bytes(format f);
		
		/// \returns the size in bytes for N indices of the specified type
		static int get_num_bytes(format f, int n);
		
	protected:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, index_buffer)
		format	m_format;
		index_buffer_storage_ptr	m_store;
		int							m_num_indices;
    };
    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, index_buffer)
    typedef scoped_buffer_lock<index_buffer_ptr> scoped_ib_lock;


} // end namespace
} // end namespace

#endif // __INDEX_BUFFER_H_37CA509A_1802_43CC_848B_EBF11A5EB338_
