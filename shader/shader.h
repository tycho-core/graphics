//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 7:00:06 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SHADER_H_9BA4DFB7_1214_4C76_8BA9_C6FBCC9A8548_
#define __SHADER_H_9BA4DFB7_1214_4C76_8BA9_C6FBCC9A8548_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/resource.h"
#include "graphics/vertex_format.h"
#include "graphics/forward_decls.h"
#include "graphics/shader/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{

	/// A shader controls all aspects of rendering geometry to the render target. It exposes
	/// its parameter's using a shader parameter table that describes all user configurable
	/// parameters. This can then by modified by the user to point to the correct places
	/// for them and used in future render calls.
    class TYCHO_GRAPHICS_ABI shader_base : public graphics::resource
    {
    public:    
		/// \returns the number of vertex input semantics this shader requires
		virtual int get_num_input_semantics() const { return 0; }
		
		/// \returns the vertex semantics required for this shader
		virtual const vertex_format::semantic_decl* get_input_semantic(int) const { return 0; }
		
		/// this exposes all configurable parameters for the shader and their default settings
		/// \returns an immutable parameter buffer for this shader.
		virtual const parameter_buffer_ptr get_shader_parameters() const = 0;
		
		/// create a new shader table populating it will default values, this can be
		/// modified by the user and used in subsequent render calls using this shader.
		/// \returns pointer to allocated buffer, user is responsible for destroying it.
		virtual parameter_buffer_ptr create_default_table() const = 0;
		
		/// \returns the parameter layout for this shader
		virtual const parameter_buffer_layout_ptr get_parameter_layout() const = 0;
		
		/// called by the renderer to configure the shader
		virtual void render_setup(parameter_buffer_ptr) = 0;
		
		/// called by the user to bind the shader to its parameters
		/// \todo it would be really good to delay this until later so it can be done on the render
		///       thread as I reckon this is going to be expensive. this should be simply really to half do.
		///       just bind the input objects but not parameters now and resolve parameters in the render thread
		///		  so most of the lookups and data copying are done in the render thread.
		void bind(parameter_buffer_ptr, parameter_object_lookup_table_ptr, parameter_bindings_ptr);
		
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, shader_base)

    };

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, shader_base)

} // end namespace
} // end namespace
} // end namespace

#endif // __SHADER_H_9BA4DFB7_1214_4C76_8BA9_C6FBCC9A8548_
