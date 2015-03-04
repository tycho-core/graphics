//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 23 April 2008 1:39:20 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PARAMETER_OBJECT_LOOKUP_TABLE_H_3C32AFA0_0C30_4078_94F5_B8A31DB92F5A_
#define __PARAMETER_OBJECT_LOOKUP_TABLE_H_3C32AFA0_0C30_4078_94F5_B8A31DB92F5A_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/shader/forward_decls.h"
#include "graphics/shader/parameter_buffer_view.h"
#include "core/containers/scoped_hash_table.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////


namespace tycho
{
namespace graphics
{
namespace shader
{

	/// Maps parameter object names to parameter buffers. As rendering progress different callers
	/// can override the current parameters by pushing their own buffer onto the table, all subsequent
	/// render operations will use this buffer until it is popped or another buffer is pushed on. After
	/// the buffer is retrieved you can then lookup the member using a \ref parameter_buffer_view object.
    class TYCHO_GRAPHICS_ABI parameter_object_lookup_table : public graphics::resource
    {
	public:
	    /// push an object parameter buffer on to the table 
	    void push(const char* name, const parameter_buffer_view &);
	    
	    /// pop the object with the passed name from the table
	    void pop(const char* name);
	    
	    /// lookup an object
	    parameter_buffer_view* find(const char* name);
	    
    private:		
			TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, parameter_object_lookup_table)
			typedef core::scoped_hash_table<const char*, parameter_buffer_view, 517, &core::char_array_equals_fn> param_table;
		
		/// parameter table mapping parameter name to its accompanying parameter buffer		
		param_table m_parameters;			    
    };
    
    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, parameter_object_lookup_table)
    
    
    /// automatically removes parameter objects on scope exit.
    class TYCHO_GRAPHICS_ABI scoped_parameter_object
    {
    public:
		scoped_parameter_object(parameter_object_lookup_table_ptr lt, const char* name, const parameter_buffer_view& view) :
			m_lt(lt),
			m_name(name)
		{
			lt->push(name, view);
		}
		
		~scoped_parameter_object()
		{
			m_lt->pop(m_name);
		}
	private:
		parameter_object_lookup_table_ptr m_lt;
		const char* m_name;
    };

} // end namespace
} // end namespace
} // end namespace


#endif // __PARAMETER_OBJECT_LOOKUP_TABLE_H_3C32AFA0_0C30_4078_94F5_B8A31DB92F5A_
