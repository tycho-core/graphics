//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 3:39:20 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "parameter_buffer_view.h"
#include "parameter_buffer_layout.h"
#include "parameter_buffer.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace shader
{

	/// constructor
	parameter_buffer_view::parameter_buffer_view(parameter_buffer_ptr buffer, parameter_buffer_layout_ptr layout) :
		m_buffer(buffer),
		m_layout(layout)
	{}

	/// \returns the underlying buffer
	const parameter_buffer_ptr parameter_buffer_view::get_buffer() const
	{
		return m_buffer;	
	}
	
	/// \returns the underlying layout
	const parameter_buffer_layout_ptr parameter_buffer_view::get_layout() const
	{
		return m_layout;		
	}			

} // end namespace
} // end namespace
} // end namespace

