//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:57:46 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDERER_H_EE84EF09_EEA6_41FD_AB41_CCD4F3F018C3_
#define __RENDERER_H_EE84EF09_EEA6_41FD_AB41_CCD4F3F018C3_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/format.h"
#include "graphics/render_interface.h"
#include "graphics/window.h"
#include "graphics/text/forward_decls.h"
#include "graphics/renderer_frame_data.h"
#include "graphics/index_buffer.h"
#include "io/forward_decls.h"
#include "io/path.h"
#include <functional>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{	

	/// main renderer interface, this is a layer over the platform specific render 
	/// interface to allow us to intercept calls and add debugging support
	class TYCHO_GRAPHICS_ABI renderer
	{		
	public:
		/// constructor
		renderer(io::interface* io, render_interface *);
		
		/// destructor
		~renderer();

		//////////////////////////////////////////////////////////////////////////
		// Device enumeration
		//////////////////////////////////////////////////////////////////////////
		
		/// query the number of adapters available
		int get_num_adapters();
		
		/// \returns detailed information about the i'th adapter
		const adapter* get_adapter(int adapter_idx);				
		
		/// initialise the renderer on the specified adapter
		bool initialise(int adapter_idx);

		/// release the interface
		bool destroy();
				
		//////////////////////////////////////////////////////////////////////////
		// Graphics object creation
		//////////////////////////////////////////////////////////////////////////

		typedef renderer_frame_data::vertex_buffer_fill_func vertex_buffer_fill_func;
		typedef renderer_frame_data::vertex_buffer_storage_fill_func vertex_buffer_storage_fill_func;
		typedef renderer_frame_data::index_buffer_fill_func index_buffer_fill_func;
		typedef renderer_frame_data::mesh_fill_func mesh_fill_func;
		typedef renderer_frame_data::texture_fill_func texture_fill_func;
		static void null_texture_fill_func(graphics::texture_ptr, char*, int) {}
		static void null_vertex_buffer_storage_fill_func(graphics::vertex_buffer_storage_ptr, void*, int) {}
		
		/// create a new window
		/// \todo creates the window immediately, should be deferred.
		window_ptr create_window(int width, int height, graphics::format, graphics::format,void *platform_specific);
						   	
		/// creates an untyped vertex buffer, this cannot be used directly but must be typed via a call to create_typed_sub_vb().
		vertex_buffer_storage_ptr create_vertex_buffer_storage(int num_bytes, int create_flags);
		
		/// creates a typed region within an untyped vertex buffer. This cannot be subsequently modified.
		vertex_buffer_ptr create_vertex_buffer(vertex_buffer_storage_ptr,
											   vertex_format* fmt, 
											   int num_verts, 
											   vertex_buffer_storage_fill_func fill_func);

		/// creates a typed region within an untyped vertex buffer that can be updated dynamically.
		vertex_buffer_ptr create_dynamic_vertex_buffer(vertex_buffer_storage_ptr,
											   vertex_format* fmt, 
											   int offset,
											   int num_verts);

		/// creates a typed region within an untyped vertex buffer.
		vertex_buffer_ptr create_vertex_buffer(vertex_buffer_storage_ptr,
											   vertex_format* fmt, 
											   int offset, 
											   int num_verts);
		
		/// convenience function to create a typed vertex buffer in single call
		vertex_buffer_ptr create_vertex_buffer(vertex_format* fmt, int num_verts, int create_flags, vertex_buffer_storage_fill_func fill_func);

		/// convenience function to create a typed vertex buffer in single call
		vertex_buffer_ptr create_vertex_buffer(vertex_format* fmt, int num_verts, int create_flags);

		/// get some index buffer bytes
		index_buffer_ptr create_index_buffer(index_buffer::format type, int num_indices, int create_flags);
				
		/// get some index buffer bytes
		index_buffer_ptr create_index_buffer(index_buffer::format type, int num_indices, int create_flags, index_buffer_fill_func fill_func);

		/// creates a mesh, simplifies creation of objects that require a single callback to update indices and vertices together.
		mesh_ptr create_mesh(vertex_format *vb_fmt, int vb_num_verts, int vb_create_flags, 
							index_buffer::format ib_type, int ib_num_indices, int ib_create_flags, 
							mesh_fill_func fill_func);							
		
		/// create a texture of the specified size / format and mips.
		texture_ptr create_texture(graphics::format format, int width, int height, int num_mips, texture_fill_func fill_func);

		/// create a texture and initialise it with the passed image data
		/// \todo should be a non member utility function
		texture_ptr create_texture(tycho::image::image_base_ptr);

		/// create a texture that can be dynamically updated. Only textures created through this interface should be passed to the
		/// update_texture function.
		texture_ptr create_dynamic_texture(graphics::format format, int width, int height);
		
		/// create a render target texture
		render_target_texture_ptr create_render_texture(graphics::format format, int width, int height);
		
		/// create a blank cube texture
		cube_texture_ptr create_cube_texture(int, int) { return 0; }
		
		/// create a new render operation object
		render_op_ptr create_render_op();
    
		//////////////////////////////////////////////////////////////////////////
		// Graphics object updating
		//////////////////////////////////////////////////////////////////////////

		/// update a region of a texture
		void update_texture(texture_ptr, int x, int y, int width, int height, texture_fill_func);
		
		/// update a vertex_buffer region
		void update_vertex_buffer(vertex_buffer_ptr, int offset, int num_bytes, vertex_buffer_fill_func);
		
		/// update a index_buffer region
		void update_index_buffer(index_buffer_ptr, int offset, int num_bytes, index_buffer_fill_func);
    
		//////////////////////////////////////////////////////////////////////////
		// Shaders 
		//////////////////////////////////////////////////////////////////////////

		/// create a new system shader
		shader::shader_base_ptr	create_system_shader(system_shader_type);
		
		/// create a new gamecube shader
		shader::gamecube_shader_ptr create_gamecube_shader();
    
		//////////////////////////////////////////////////////////////////////////
		// Scene rendering
		//////////////////////////////////////////////////////////////////////////
		
		/// must be called before rendering to the passed render target
		void set_render_target(render_target_ptr);
		
		/// set the viewport 
		void set_viewport(const graphics::viewport& vp);
		
		/// submit a render operation to the driver
		void render(render_op_ptr);

		/// mark the render target to shown on a physical display
		void present(render_target_ptr);
		
		//////////////////////////////////////////////////////////////////////////
		// Frame management
		//////////////////////////////////////////////////////////////////////////
		
		/// flush all queued data to the GPU.
		bool flush(); 

		/// flip to front buffer
		bool flip();		

		/// stall until all current rendering operations are complete
		void sync();

		//////////////////////////////////////////////////////////////////////////
		// Misc
		//////////////////////////////////////////////////////////////////////////

		/// \returns the font manager
		text::font_manager& get_font_manager();
		
		/// allocate some memory for the duration of the frame
		void* frame_allocate(size_t n);
		
		/// allocate some vb space for the duration of the frame
		vertex_buffer_ptr frame_allocate_vertex_buffer(vertex_format *fmt, int num_verts);
		
		/// \returns an index list for rendering quads of vertices
		index_buffer_ptr get_quad_index_buffer(int num_quads);		

		//////////////////////////////////////////////////////////////////////////
		// Debugging
		//////////////////////////////////////////////////////////////////////////
		
		/// marks the beginning of a GPU event.
		virtual void begin_gpu_event(const char* name);
		
		/// marks the end of a GPU event
		virtual void end_gpu_event();
    
    public:
		/// \name Render thread functions, attempts to call from any other thread will cause an assert.
		//@{	
		void rt_update_meshes(renderer_frame_data* frame);
		void rt_update_vertex_buffers(renderer_frame_data* frame);
		void rt_update_index_buffers(renderer_frame_data* frame);
		void rt_update_textures(renderer_frame_data* frame);
		
		void rt_create_vertex_buffer_storage(renderer_frame_data* frame, renderer_frame_data::create_vertex_buffer_storage& create);
		void rt_create_index_buffer_storage(renderer_frame_data* frame, renderer_frame_data::create_index_buffer_storage& create);
		void rt_update_vertex_buffer_storage(renderer_frame_data* frame, renderer_frame_data::update_vertex_buffer& update);
		void rt_update_index_buffer_storage(renderer_frame_data* frame, renderer_frame_data::update_index_buffer& update);
		void* rt_get_index_buffer_out_buffer(renderer_frame_data* frame, renderer_frame_data::create_index_buffer_storage& create);
		void* rt_get_vertex_buffer_out_buffer(renderer_frame_data* frame, renderer_frame_data::create_vertex_buffer_storage& create);		
		
		/// checks that it is called from the render thread. asserts if not.
		void rt_check_thread(); 
		
		/// process and render a frames worth of data
		void rt_process_frame(renderer_frame_data*);
		
		/// Process render target. This performs non rendering tasks and can run concurrently with
		/// other render target processing so cannot access the underlying hardware device.
		void rt_process_render_target(renderer_frame_data::render_target_data& rt);
		
		/// render to the render target.
		void rt_render_render_target(renderer_frame_data::render_target_data& rt);
		//@}				
		
	private:
		static const int NumFrames = 2;
		static const int NumQuads;
		typedef std::map<io::path, texture_ptr> texture_cache;
				
        io::interface*        m_io;
		render_interface*	  m_interface;			   ///< underlying render device interface
		texture_cache		  m_texture_cache;         ///< cache of all loaded textures
		text::font_manager*   m_font_manager;          ///< text rendering services
		renderer_frame_data*  m_frame_data[NumFrames]; ///< frame render data buffers.
		renderer_frame_data*  m_cur_render_frame;      ///< render data currently being process by the render thread
		renderer_frame_data*  m_cur_update_frame;      ///< render data currently being added to be a non-render thread
		index_buffer_ptr	  m_quad_index_list;	   ///< index list for rendering quads of vertices
	};

} // end namespace
} // end namespace

#endif // __RENDERER_H_EE84EF09_EEA6_41FD_AB41_CCD4F3F018C3_
