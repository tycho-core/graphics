//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:59:25 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VERTEX_BUFFER_H_77141AD3_42BC_4F8C_82B1_06DE4A070502_
#define __VERTEX_BUFFER_H_77141AD3_42BC_4F8C_82B1_06DE4A070502_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/resource.h"
#include "graphics/forward_decls.h"
#include "graphics/vertex_buffer_storage.h"
#include "graphics/vertex_format.h"
#include "graphics/scoped_buffer_lock.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	class vertex_buffer_storage;
	
	/// interface to GPU vertex buffer. this types (through the vertex_format desc) the underlying
	/// raw storage buffer.
    class TYCHO_GRAPHICS_ABI vertex_buffer : public graphics::resource
    {
    public:
		/// flags to help guide the driver when allocating it
		enum create_flags
		{
			cf_static, ///< buffer will be initialised once then not modified.
			cf_dynamic ///< buffer will be updated frequently
		};
        
    public:
		/// constructor
		vertex_buffer(vertex_format*, int num_verts);
		
		/// create the vertex buffer, only valid from the render thread
		bool create(vertex_format*, vertex_buffer_storage_ptr);
		
		/// destructor
		~vertex_buffer();
    
		/// lock the buffer
		void* lock();
		
		/// unlock the buffer
		void unlock();
		
		/// attempt to resize the buffer
		bool resize(int num_verts, bool preserve);
		
		/// format of the vertex buffer
		const vertex_format* get_format() const;
			
		/// get the size of the vertex buffer in bytes
		int get_bytes_used() const;
		
		/// \returns the number of vertices there is space for in the buffer
		int get_num_verts() const;
		
		/// \returns the offset of the VB in the underlying buffer
		int get_buffer_offset() const;
		
		/// \returns underlying storage
		vertex_buffer_storage_ptr get_storage() 
			{ return m_store; }
			
		/// set the underlying storage
		void set_storage(vertex_buffer_storage_ptr s)
			{ m_store = s; }			
			
		/// copy the input vertices to the output possibly converting between types
		static bool write(const vertex_format &src_format, const void* src_ptr, const vertex_format &dst_format, void* dst_ptr, int num_verts);
		
		
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, vertex_buffer)
		
	protected:
		vertex_format*              m_format;
		vertex_buffer_storage_ptr	m_store;
		int							m_num_verts;
    };

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, vertex_buffer)
	typedef scoped_buffer_lock<vertex_buffer_ptr> scoped_vb_lock;

} // end namespace
} // end namespace

#endif // __VERTEX_BUFFER_H_77141AD3_42BC_4F8C_82B1_06DE4A070502_
