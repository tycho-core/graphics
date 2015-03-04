//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 2:25:23 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PARAMETER_BUFFER_H_F4A0DDD9_554E_4DDB_AD8E_85FD2AB650B8_
#define __PARAMETER_BUFFER_H_F4A0DDD9_554E_4DDB_AD8E_85FD2AB650B8_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/shader/forward_decls.h"
#include "graphics/resource.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{
	/// this holds the memory buffer storing a set of parameters.
    class TYCHO_GRAPHICS_ABI parameter_buffer : public graphics::resource
    {
    public:
		/// constructor
		parameter_buffer(int size);
		
		/// destructor
		~parameter_buffer();
		
		/// \returns a pointer to the raw buffer
		core::uint8* ptr()
			{ return m_buffer; }
			
		/// \returns a pointer to the raw buffer
		const core::uint8* ptr() const
			{ return m_buffer; }
		
		/// \returns an exact copy of the buffer
		parameter_buffer_ptr clone() const;
		
    private:
		/// non copyable
		void operator=(const parameter_buffer&);
		
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, parameter_buffer)
		
		core::uint8* m_buffer;	///< raw bytes
		int   m_size;	///< size in bytes.
    };

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, parameter_buffer);

} // end namespace
} // end namespace
} // end namespace

#endif // __PARAMETER_BUFFER_H_F4A0DDD9_554E_4DDB_AD8E_85FD2AB650B8_
