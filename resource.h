//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 7:02:53 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RESOURCE_H_A07A05AA_6C38_486A_9AAF_08B7A93C9A17_
#define __RESOURCE_H_A07A05AA_6C38_486A_9AAF_08B7A93C9A17_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "core/intrusive_ptr.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	
	/// Base class for all graphics resources (which is most things). All resources
	/// a reference counted and intended to be used with boost::intrusive_ptr's.
    class TYCHO_GRAPHICS_ABI resource
    {
    public:
		enum state
		{
			s_loading,		///< resource is currently not initialised by the underlying render device
			s_created,		///< resource has been initialised by the underlying render device 
			s_initialised	///< resource had been written to.
		};
		
    public:
		/// default constructor
		resource() : m_ref_count(0), m_state(s_loading) {}
		
		/// destructor
		virtual ~resource() { TYCHO_ASSERT(m_ref_count == 0); }
		
		/// \returns if the resource is fully loaded
		bool is_loaded() const
			{ return get_state() != s_loading; }

		/// \returns the current state of the resource
		state get_state() const
			{ return m_state; }
		
		/// sets the current state of the resource
		void set_state(state s) 
			{ m_state = s; }
						
	private:
		// resources are non copyable
		resource& operator=(const resource &);
		
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, resource)
				
		int m_ref_count;
		state m_state;
		
	protected:
		void add_ref() { ++m_ref_count; }
		void dec_ref() { --m_ref_count;  if(m_ref_count == 0) delete this;	}	
    };

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, resource)

} // end namespace
} // end namespace

#endif // __RESOURCE_H_A07A05AA_6C38_486A_9AAF_08B7A93C9A17_
