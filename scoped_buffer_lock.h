//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 20 May 2008 2:04:36 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SCOPED_BUFFER_LOCK_H_C76B79FC_2301_47BF_99A1_900274A7AC2E_
#define __SCOPED_BUFFER_LOCK_H_C76B79FC_2301_47BF_99A1_900274A7AC2E_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	template<class T>
    struct scoped_buffer_lock
    {
		scoped_buffer_lock(T b) : buffer(b), ptr(0) { ptr = buffer->lock(); TYCHO_ASSERT(ptr); }
		~scoped_buffer_lock() { buffer->unlock(); }
		
		T buffer;
		void* ptr;
    };

} // end namespace
} // end namespace

#endif // __SCOPED_BUFFER_LOCK_H_C76B79FC_2301_47BF_99A1_900274A7AC2E_
