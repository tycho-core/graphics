//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 3 April 2008 11:13:11 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SYSMEM_VERTEX_BUFFER_H_33CA7F6C_E3B8_45F0_A743_1EFDC7F16F67_
#define __SYSMEM_VERTEX_BUFFER_H_33CA7F6C_E3B8_45F0_A743_1EFDC7F16F67_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/vertex_buffer_storage.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// system memory vertex buffer. 
    class TYCHO_GRAPHICS_ABI sysmem_vertex_buffer : public graphics::vertex_buffer_storage
    {
    public:
		/// constructor
		sysmem_vertex_buffer();
		
		/// destructor()
		~sysmem_vertex_buffer();
		
		/// lock the buffer and write some data to it
		virtual void* lock();
		
		/// unlock the buffer
		virtual void unlock();
		
		/// size of the buffer
		virtual int size();
		
		/// resize the buffer, may not be supported by some classes.
		virtual bool resize(int size, bool preserve);

	private:
	    char*	m_data;
	    int		m_size;
	    bool	m_locked; ///< check for recursive locks
    };

} // end namespace
} // end namespace

#endif // __SYSMEM_VERTEX_BUFFER_H_33CA7F6C_E3B8_45F0_A743_1EFDC7F16F67_
