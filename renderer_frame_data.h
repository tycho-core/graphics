//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 30 June 2008 12:18:54 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDERER_FRAME_DATA_H_66C19C7B_D176_461F_AD76_0A0180DD27D4_
#define __RENDERER_FRAME_DATA_H_66C19C7B_D176_461F_AD76_0A0180DD27D4_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/viewport.h"
#include "graphics/format.h"
#include "graphics/index_buffer.h"
#include "graphics/buffer_storage.h"
#include "graphics/mesh.h"
#include "image/forward_decls.h"
#include "core/memory/grow_only_pod_allocator.h"
#include "boost/function.hpp"
#include <vector>
#include <map>
#include <functional>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// complete set of data for rendering a single frame. this includes lists of all updates
	/// to made to existing resources (index, vertex and textures) as well as a list of objects
	/// to create the render side data for. Resource creation is deferred so it can occur in the
	/// render thread to avoid synchronization.
	struct TYCHO_GRAPHICS_ABI renderer_frame_data
	{
	private:
        friend class renderer;
		
		/// constructor.
		renderer_frame_data(renderer*);
		
		~renderer_frame_data();
		
		void initialise();

		/// resets to default
		void clear();	
		
		/// complete set of data for rendering a single render target
		struct render_target_data
		{
			render_target_data() {}
			render_target_data(render_target_ptr rt_) : rt(rt_) {}
			
			/// per viewport render data
			struct viewport_data
			{
				viewport_data() {}
				viewport_data(const viewport& vp_) : vp(vp_) {}
				
				graphics::viewport			vp;	  ///< viewport to render to
				std::vector<render_op_ptr>  ops;  ///< render operations to perform on the render target				
			};
			
			graphics::render_target_ptr rt;	  ///< render target to render to
			std::vector<viewport_data>		viewports;
		};		

		typedef std::function<void(graphics::vertex_buffer_ptr, void* ptr, int buffer_size)> vertex_buffer_fill_func;
		typedef std::function<void(graphics::vertex_buffer_storage_ptr, void* ptr, int buffer_size)> vertex_buffer_storage_fill_func;
		typedef std::function<void(graphics::index_buffer_storage_ptr, void* ptr, int buffer_size)> index_buffer_fill_func;
		typedef std::function<void(graphics::mesh_ptr, void* vb_ptr, int vb_size, void* ib_ptr, int ib_size)> mesh_fill_func;
		typedef std::function<void(graphics::texture_ptr, char* buffer, int stride)> texture_fill_func;
				
		enum resource_type
		{
			resource_type_dynamic,
			resource_type_static			
		};
		
		/// vertex buffer management
		
		struct create_vertex_buffer_storage
		{
			vertex_buffer_storage_ptr store;
			vertex_buffer_storage_fill_func user_fn;			
			int						  num_bytes;
			int					      create_flags;
			resource_type			  type;
		};		
		
		struct update_vertex_buffer
		{
			vertex_buffer_ptr	vb;
			int					offset;
			int					num_bytes;
			vertex_buffer_fill_func	user_fn;
		};
				
		/// index buffer management
		
		struct create_index_buffer_storage
		{
			index_buffer_storage_ptr store;
			index_buffer_fill_func   user_fn;
			index_buffer::format     format;
			int						 num_bytes;
			int						 num_indices;
			int						 create_flags;
			resource_type			 type;
		};

		struct update_index_buffer
		{
			index_buffer_ptr	ib;
			int					offset;
			int					num_bytes;
			index_buffer_fill_func user_fn;
		};
		
		/// texture management
		struct create_texture
		{
			texture_ptr			texture;
			texture_fill_func   user_fn;
			image::image_base_ptr image;   ///< used if non null
			format				texture_format;
			int					width;
			int					height;
			int					num_mips;			
			resource_type		type;
		};		

		struct update_texture
		{
			texture_ptr texture;
			int dst_x;
			int dst_y;
			int width;
			int height;
			texture_fill_func user_fn;
		};
		
		/// render to texture management
		
		struct create_render_texture
		{
			render_target_texture_ptr texture;
			graphics::format texture_format;
			int width;
			int height;			
		};
		
		/// mesh management
		
		struct create_mesh
		{
			mesh_ptr	   mesh;
			mesh_fill_func user_fn;
			create_vertex_buffer_storage cvb;
			create_index_buffer_storage  cib;			
		};
		
						
		/// \name Resource creation and update callbacks
		//@{
		std::vector<create_index_buffer_storage>  ib_creates;
		std::vector<update_index_buffer>		  ib_updates;
		std::vector<create_vertex_buffer_storage> vb_creates;
		std::vector<update_vertex_buffer>		  vb_updates;
		std::vector<create_texture>				  texture_creates;
		std::vector<update_texture>				  texture_updates;
		std::vector<create_render_texture>		  rtt_creates;
		std::vector<create_mesh>				  mesh_creates;
		//@}				
		
		/// renderer this frame data is associated with
		graphics::renderer* m_renderer;
		
		/// list of all render targets that need updating this frame
		std::vector<render_target_data> render_targets;
		
		/// list of all render targets to present to screen
		std::vector<render_target_ptr> present_render_targets;
		
		/// Grow only memory pool for this frames dynamic data. Any allocations that are required to survive until
		/// rendering is complete should come from here, this is a POD only pool so it just pulled down at the end
		/// of frame without calling any destructors.
		core::grow_only_pod_allocator allocator;
		
		/// dynamic vertex buffers for single frame allocation
		vertex_buffer_storage_ptr m_frame_vertex_buffer;
		
		/// number of bytes currently allocated from the frame vertex buffer
		int m_vertex_bytes_allocated;
		
		/// dynamic index buffers for single frame allocation
		index_buffer_ptr m_frame_index_buffer;		
		
		struct locked_vb
		{
			bool operator<(const locked_vb& rhs) const 
			{
				return store < rhs.store;
			}
			vertex_buffer_storage_ptr store;
			void*					  out_buffer;
		};
		typedef std::map<buffer_storage::driver_object_ptr, locked_vb> unique_vbs_t;

		struct locked_ib
		{
			bool operator<(const locked_ib& rhs) const 
			{
				return store < rhs.store;
			}
			index_buffer_storage_ptr store;
			void*					 out_buffer;
		};
		typedef std::map<buffer_storage::driver_object_ptr, locked_ib> unique_ibs_t;
		
		unique_vbs_t unique_vbs;
		unique_ibs_t unique_ibs;
	};	
	
} // end namespace
} // end namespace

#endif // __RENDERER_FRAME_DATA_H_66C19C7B_D176_461F_AD76_0A0180DD27D4_
