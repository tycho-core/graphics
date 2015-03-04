//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 3:39:20 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PARAMETER_BUFFER_VIEW_H_F6FA6D15_4B24_43C0_82D4_8FA8CC271587_
#define __PARAMETER_BUFFER_VIEW_H_F6FA6D15_4B24_43C0_82D4_8FA8CC271587_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/shader/forward_decls.h"
#include "graphics/shader/parameter_buffer_layout.h"
#include "graphics/shader/parameter_buffer.h"
#include "core/colour/rgba.h"
#include "core/memory.h"
#include "core/string.h"
#include "math/matrix4x4.h"
#include "math/vector4.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{

	/// A view is lightweight object used to map a particular parameter layout over a parameter
	/// buffer. This takes care of all arithmetic to get and set the parameters and also adds
	/// added checking in debug mode that parameters and types match correctly, in release it 
	/// assumes they do and no checking is performed. 	
    class TYCHO_GRAPHICS_ABI parameter_buffer_view
    {
    public:
		/// constructor
		/// \param buffer raw buffer where params are stored
		/// \param layout layout of the buffer
		parameter_buffer_view(parameter_buffer_ptr buffer, parameter_buffer_layout_ptr layout);		
		
		/// copy constructor
		parameter_buffer_view(const parameter_buffer_view& other) :
			m_buffer(other.m_buffer),
			m_layout(other.m_layout)
		{}
				
		/// default constructor
		parameter_buffer_view() {}
		
		/// assignment operator, performs a shallow copy of the of view.
		parameter_buffer_view& operator=(const parameter_buffer_view &rhs)
		{
			m_buffer = rhs.m_buffer;
			m_layout = rhs.m_layout;
			return *this;
		}		
				
		/// \returns the underlying buffer
		const parameter_buffer_ptr get_buffer() const;
		
		/// \returns the underlying layout
		const parameter_buffer_layout_ptr get_layout() const;
		
		/// retrieve a parameter by name. 
		template<class T> T get(const char *name)
		{
			T *p = get_ptr<T>(name);
			TYCHO_ASSERT(p);
			return *p;	
		}

		/// set a parameter by name. 
		/// \todo need to use copy constructor and delete previously held one.
		template<class T> parameter_buffer_view set(const char *name, const T &v)
		{
		    int offset = m_layout->get_param_offset(name, core::strlen(name));
			if(offset == -1)
				return *this;
			
			//!! unsafe for non pod types
			core::mem_cpy((char*)m_buffer->ptr() + offset, &v, sizeof(T));		
			return *this;
		}

		/// set a graphics::resource parameter by name. 
// 		parameter_buffer_view set(const char *name, const graphics::resource &v)
// 		{
// 			int offset = m_layout->get_param_offset(name, strlen(name));
// 			if(offset == -1)
// 				return *this;
// 			
// 			v.add_ref();
// 			(graphics::resource*)(m_buffer->ptr() + offset) = &v;
// 			return *this;
// 		}

		/// retrieve a pointer to the parameter by name
		template<class T> T* get_ptr(const char *name)
		{
		    int offset = m_layout->get_param_offset(name, core::strlen(name));
			if(offset == -1)
				return 0;
			T* val = reinterpret_cast<T*>((char*)m_buffer->ptr() + offset);
			return val;				
		}

		/// retrieve a pointer to the parameter by index
		template<class T> T* get_ptr(int index)
		{
			int offset = m_layout->get_param_offset(index);
			if(offset == -1)
				return 0;
			T* val = reinterpret_cast<T*>((char*)m_buffer->ptr() + offset);
			return val;				
		}
				
    private:
		parameter_buffer_ptr		m_buffer;  ///< raw byte buffer where params are stored
		parameter_buffer_layout_ptr	m_layout;  ///< layout of the params in the buffer
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __PARAMETER_BUFFER_VIEW_H_F6FA6D15_4B24_43C0_82D4_8FA8CC271587_
