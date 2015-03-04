//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 23 April 2008 1:39:21 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "parameter_object_lookup_table.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace shader
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, parameter_object_lookup_table);

    /// push an object parameter buffer on to the table 
    void parameter_object_lookup_table::push(const char* name, const parameter_buffer_view &view)
    {
		m_parameters.push(name, view);
    }
    
    /// pop the object with the passed name from the table
    void parameter_object_lookup_table::pop(const char* name)
    {
		m_parameters.pop(name);
    }
    
    /// lookup an object
    parameter_buffer_view* parameter_object_lookup_table::find(const char* name)
    {
		return m_parameters.find(name);
    }

} // end namespace
} // end namespace
} // end namespace
