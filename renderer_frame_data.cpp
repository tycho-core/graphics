//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 30 June 2008 12:18:55 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/renderer_frame_data.h"
#include "graphics/renderer.h"
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, resource);

	static const int AllocatorPageSize = 16 * 1024; 
	static const int DynamicVertexBufferSize = 64 * 1024;
    //static const int DynamicVertexBufferCount = 1;
	static const int DynamicIndexBufferSize = 64 * 1024;
    //static const int DynamicIndexBufferCount = 1;
	
	renderer_frame_data::renderer_frame_data(renderer* r) :
		m_renderer(r),
		allocator(AllocatorPageSize),
		m_vertex_bytes_allocated(0)
	{
	}

	renderer_frame_data::~renderer_frame_data()
	{
	}
	
	void renderer_frame_data::initialise()
	{
		// allocate dynamic vertex and index buffers used for temporary space for the lifetime of the frame
		m_frame_vertex_buffer = m_renderer->create_vertex_buffer_storage(DynamicVertexBufferSize, vertex_buffer::cf_dynamic);
		m_frame_index_buffer = m_renderer->create_index_buffer(index_buffer::f_int16, DynamicIndexBufferSize / 2, index_buffer::cf_dynamic);	
	}
	
	void renderer_frame_data::clear()
	{
		ib_creates.clear();
		ib_updates.clear();
		vb_creates.clear();
		vb_updates.clear();
		mesh_creates.clear();
		texture_creates.clear();
		texture_updates.clear();
		render_targets.clear();		
		present_render_targets.clear();	
		rtt_creates.clear();
		allocator.release_all();
		unique_vbs.clear();
		unique_ibs.clear();
		m_vertex_bytes_allocated = 0;
	}

} // end namespace
} // end namespace
