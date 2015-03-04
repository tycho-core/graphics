//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 9:03:40 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDER_INTERFACE_H_6E6BF4E3_E652_42F3_B015_27F045219CAE_
#define __RENDER_INTERFACE_H_6E6BF4E3_E652_42F3_B015_27F045219CAE_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/shader/forward_decls.h"
#include "graphics/format.h"
#include "graphics/index_buffer.h" // for index_buffer::format
#include "graphics/vertex_buffer_storage.h" // for vertex_buffer_storage::driver_object
#include "graphics/index_buffer_storage.h" // for index_buffer_storage::driver_object
#include "image/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	/// basic system shader types
	enum system_shader_type
	{
		/// system font shader
		system_shader_font,
		
		/// render a mesh with a single color per vertex
		system_shader_single_clr,
		
		/// ui shader, no lighting and single texture
		system_shader_ui,
		
		// ui string shader
		system_shader_ui_string
	};
	
	/// defines the interface for platform specific render drivers
	class TYCHO_GRAPHICS_ABI render_interface
	{
	public:
		/// virtual destructor
		virtual ~render_interface() {}
		
		//////////////////////////////////////////////////////////////////////////
		// Render interface initialisation
		//////////////////////////////////////////////////////////////////////////

		/// get the number of adapters attached to this interface
		virtual int get_num_adapters() = 0;
		
		/// \returns detailed information about the i'th adapter
		virtual const adapter* get_adapter(int i) = 0;

		/// initialise the renderer on the specified adapter
		virtual bool initialise(int adapter_idx) = 0;

		/// release the interface
		virtual bool destroy() = 0;

		//////////////////////////////////////////////////////////////////////////
		// Graphics object creation
		//////////////////////////////////////////////////////////////////////////
		
		/// create a new window
		virtual window_storage*	create_window(int width, int height, graphics::format, graphics::format, void *platform_data) = 0;
		
		/// create a texture and initialise it with the passed image data
		virtual texture_storage* create_texture(tycho::image::image_base_ptr) = 0;
		
		/// create a texture of the specified size / format and mips.
		virtual texture_storage* create_texture(graphics::format format, int width, int height, int num_mips) = 0;

		/// create a dynamic texture of the specified size / format. No mip maps are allowed.
		virtual texture_storage* create_dynamic_texture(graphics::format format, int width, int height) = 0;

		/// create a render target texture of the specified size / format. No mip maps are allowed.
		virtual render_target_texture_storage* create_render_texture(graphics::format format, int width, int height) = 0;
		
		/// get some vertex buffer bytes
		virtual vertex_buffer_storage::driver_object* create_vertex_buffer(int num_bytes, int alignment, int create_flags) = 0;
		
		/// get some index buffer bytes
		virtual index_buffer_storage::driver_object* create_index_buffer(index_buffer::format type, int num_indices, int create_flags) = 0;
							
		/// create a new render operation object
		virtual render_op* create_render_op() = 0;
		
		//////////////////////////////////////////////////////////////////////////
		// Shaders :
		// We use explicit creation functions for all shader types, there will only
		// be a few at this level as they themselves expose a configurable interface
		// and these provide the back end support for those on a particular platform
		// if it is supported.
		//////////////////////////////////////////////////////////////////////////

		/// built in system shaders
		virtual shader::shader_base_ptr create_system_shader(system_shader_type) = 0;

		/// create a new gamecube shader
		virtual shader::gamecube_shader_ptr create_gamecube_shader() = 0;

		//////////////////////////////////////////////////////////////////////////
		// Scene rendering
		//////////////////////////////////////////////////////////////////////////
		
		/// must be called before rendering to the passed render target
		virtual void begin_render(render_target_ptr) = 0;
		
		/// must be called when rendering to the render target is done
		virtual void end_render() = 0;
		
		/// set the viewport 
		virtual void set_viewport(const graphics::viewport& vp) = 0;

		/// submit a render operation to the driver
		virtual void render(render_op_ptr) = 0;

		//////////////////////////////////////////////////////////////////////////
		// Frame management
		//////////////////////////////////////////////////////////////////////////
		
		/// flush all queued data to the GPU.
		virtual bool flush() = 0; 

		/// flip to front buffer
		virtual bool flip() = 0;		

		/// stall until all current rendering operations are complete
		virtual void sync() = 0;


		//////////////////////////////////////////////////////////////////////////
		// Debugging
		//////////////////////////////////////////////////////////////////////////
		
		/// marks the beginning of a GPU event.
		virtual void begin_gpu_event(const char* /*name*/) {}
		
		/// marks the end of a GPU event
		virtual void end_gpu_event() {}
	};

} // end namespace
} // end namespace

#endif // __RENDER_INTERFACE_H_6E6BF4E3_E652_42F3_B015_27F045219CAE_
