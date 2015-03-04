//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:59:53 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __CAMERA_H_29744AB0_16CB_4F7E_A784_1A8C840FB326_
#define __CAMERA_H_29744AB0_16CB_4F7E_A784_1A8C840FB326_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/viewport.h"
#include "graphics/resource.h"
#include "graphics/forward_decls.h"
#include "graphics/shader/forward_decls.h"
#include "math/matrix4x4.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// contains scene wide rendering properties
    class TYCHO_GRAPHICS_ABI camera : public graphics::resource
    {
    public:
		enum matrix_type
		{
			matrix_type_view,
			matrix_type_proj,
			matrix_type_view_proj,
			matrix_type_count
		};
		
    public:
		/// constructor
		camera();
		
		/// set the render target for this camera
		void set_render_target(render_target_ptr);
		
		/// set the viewport for this camera
		void set_viewport(const viewport &);
		
		/// set a matrix
		void set_matrix(matrix_type t, const math::matrix4x4f &);
		
		/// set the object parameter buffer to use for this camera
		void set_parameter_object_lookup_table(shader::parameter_object_lookup_table_ptr);

		/// \returns the render target for this camera
		render_target_ptr get_render_target();
		
		/// \returns the viewport for this camera
		viewport get_viewport();
				
		/// \returns a camera matrix
		const math::matrix4x4f& get_matrix(matrix_type t);

		/// \returns the object parameter buffer to use for this camera
		shader::parameter_object_lookup_table_ptr get_parameter_object_lookup_table();
		
		/// \returns parameter view for this camera
		shader::parameter_buffer_view get_parameter_buffer_view();

    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, camera)

		render_target_ptr					m_render_target;
		viewport							m_viewport;
		shader::parameter_buffer_ptr		m_buffer;
		shader::parameter_buffer_layout_ptr m_layout;
		shader::parameter_object_lookup_table_ptr m_input_params;
    };

	//TYCHO_INTRUSIVE_PTR_FUNC_DECLS(camera)
	TYCHO_GRAPHICS_ABI void intrusive_ptr_add_ref(camera *r);
	TYCHO_GRAPHICS_ABI void intrusive_ptr_release(camera *r);
	
} // end namespace
} // end namespace

#endif // __CAMERA_H_29744AB0_16CB_4F7E_A784_1A8C840FB326_
