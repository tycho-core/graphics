//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 9:26:17 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDER_OP_H_A3FC9C22_CEC5_453F_A9DC_D8781495C364_
#define __RENDER_OP_H_A3FC9C22_CEC5_453F_A9DC_D8781495C364_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/shader/forward_decls.h"
#include "graphics/shader/shader.h"
#include "graphics/resource.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// primitive rendering topology type.
	enum primitive_type
	{
		primitive_type_invalid = 0,
		primitive_type_tri_list,
		primitive_type_tri_strip,
		primitive_type_line_list,
		primitive_type_line_strip,		
	};
	
	/// a render operation encompasses all information required to make a single
	/// render call. 
	class TYCHO_GRAPHICS_ABI render_op : public graphics::resource
	{
	public:
		static const int MaxVertexBuffers = 4;
		static const int MaxIndexBuffers = 4;
		
		/// default constructor
		render_op();
		
		/// vertex buffers to bind for this call		
		vertex_buffer_ptr vertex_buffers[MaxVertexBuffers];
		
		/// index buffer to bind for this call
		index_buffer_ptr index_buffers[MaxIndexBuffers];
				
		/// shader to render with
		shader::shader_base_ptr shader;        
		
		/// parameters for the shader
		shader::parameter_buffer_ptr shader_params;
		
		/// number of vertices in call
		int num_vertices;
		
		/// first vertex to use
		int first_vertex;
		
		/// number of indices in the call
		int num_indices;
		
		/// first index to use
		int first_index;

		/// primitive type to use
		primitive_type primitive;
				
	private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, render_op);
		friend class renderer;
		bool disabled;
	};

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, render_op)

} // end namespace
} // end namespace

#endif // __RENDER_OP_H_A3FC9C22_CEC5_453F_A9DC_D8781495C364_
