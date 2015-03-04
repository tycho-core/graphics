//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 4 April 2008 2:00:52 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "vertex_buffer_view.h"
#include "vertex_buffer.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{

	/// constructor
	vertex_buffer_view::vertex_buffer_view(vertex_buffer *vb)
	{
	}	
	
	/// destructor
	vertex_buffer_view::~vertex_buffer_view()
	{}

	void vertex_buffer_view::write(vertex_format::semantic_type /*semantic*/, char)
	{
	
	}
	
	void vertex_buffer_view::write(vertex_format::semantic_type, float)
	{
	
	}
	
	void vertex_buffer_view::write(vertex_format::semantic_type, int)
	{
	
	}
	
	void vertex_buffer_view::write(vertex_format::semantic_type, double)
	{
	
	}
	
	void vertex_buffer_view::write(vertex_format::semantic_type, math::vector3f)
	{
	
	}
	

} // end namespace
} // end namespace
