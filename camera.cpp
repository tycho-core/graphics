//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:59:53 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/camera.h"
#include "graphics/shader/parameter_buffer_view.h"
#include "graphics/shader/parameter_buffer_layout.h"
#include "graphics/shader/parameter_buffer.h"
#include "graphics/shader/parameter_object_lookup_table.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	using namespace shader;

	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, camera);
		
	static const char* to_param_name(camera::matrix_type t)
	{
		switch(t)
		{
			case camera::matrix_type_view : return "view_matrix";
			case camera::matrix_type_proj : return "proj_matrix";
			case camera::matrix_type_view_proj : return "view_proj_matrix";
                
            default : break;
		}	
		return "";
	}

	camera::camera()
	{
		m_layout = parameter_buffer_layout_ptr(new parameter_buffer_layout());
		m_layout->add_parameter("view_matrix", spt_matrix44, spm_none);
		m_layout->add_parameter("proj_matrix", spt_matrix44, spm_none);
		m_layout->add_parameter("view_proj_matrix", spt_matrix44, spm_none);
		m_layout->finalise();
		m_buffer = parameter_buffer_ptr(new parameter_buffer(m_layout->get_required_size()));
	}

	/// set the render target for this camera
	void camera::set_render_target(render_target_ptr rt)
	{
		m_render_target = rt;
	}
	
	/// set the viewport for this camera
	void camera::set_viewport(const viewport &vp)
	{
		m_viewport = vp;
	}
	
	/// set a matrix
	void camera::set_matrix(matrix_type t, const math::matrix4x4f &m)
	{
		/// \todo to cache matrix pointers and avoid lookup
		parameter_buffer_view view(m_buffer, m_layout);
		view.set<math::matrix4x4f>(to_param_name(t), m);				
	}
	
	/// set the object parameter buffer to use for this camera
	void camera::set_parameter_object_lookup_table(shader::parameter_object_lookup_table_ptr p)
	{
		m_input_params = p;
	}

	/// \returns the render target for this camera
	render_target_ptr camera::get_render_target()
	{
		return m_render_target;
	}
	
	/// \returns the viewport for this camera
	viewport camera::get_viewport()
	{
		return m_viewport;
	}
			
	/// \returns a camera matrix
	const math::matrix4x4f& camera::get_matrix(matrix_type t)
	{
		/// \todo to cache matrix pointers and avoid lookup
		parameter_buffer_view view(m_buffer, m_layout);
		return *view.get_ptr<math::matrix4x4f>(to_param_name(t));				
	}

	/// \returns the object parameter buffer to use for this camera
	shader::parameter_object_lookup_table_ptr camera::get_parameter_object_lookup_table()
	{
		return m_input_params;
	}

	/// \returns parameter view for this camera
	shader::parameter_buffer_view camera::get_parameter_buffer_view()
	{
		return graphics::shader::parameter_buffer_view(m_buffer, m_layout);
	}

} // end namespace
} // end namespace
