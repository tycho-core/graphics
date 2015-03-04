//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 23 April 2008 1:56:01 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "parameter_bindings.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace shader
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, parameter_bindings);

	void parameter_bindings::bind(const char* shader_param, const char* src_object, const char* src_param)
	{
		parameter_binding nb;
		nb.shader_param = shader_param;
		nb.src_object = src_object;
		nb.src_param = src_param;
		m_bindings.push_back(nb);
	}
	
	int parameter_bindings::get_num_bindings() const
	{
		return static_cast<int>(m_bindings.size());
	}
	
	const parameter_binding& parameter_bindings::get_binding(int i) const
	{
		TYCHO_ASSERT(i < get_num_bindings());
		return m_bindings[i];
	}

	parameter_binding& parameter_bindings::get_binding(int i)
	{
		TYCHO_ASSERT(i < get_num_bindings());
		return m_bindings[i];	
	}

} // end namespace
} // end namespace
} // end namespace

