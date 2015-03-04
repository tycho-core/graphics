//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 3 April 2008 11:17:12 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VERTEX_BUFFER_STORAGE_H_B105D5EA_0949_4365_A66A_5DD945D5B951_
#define __VERTEX_BUFFER_STORAGE_H_B105D5EA_0949_4365_A66A_5DD945D5B951_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/resource.h"
#include "graphics/buffer_storage.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// abstraction of vertex buffer storage. this could be in GPU memory or in a system
	/// memory buffer or an elaborate one could virtualise access to a massive chunk
	/// of terrain vertices's.
    class TYCHO_GRAPHICS_ABI vertex_buffer_storage : public graphics::buffer_storage
    {    
    public:
		/// constructor
		vertex_buffer_storage(int num_bytes, int create_flags);

    protected:
        int get_create_flags() const;

    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, vertex_buffer_storage)		
		int m_create_flags;
    };

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, vertex_buffer_storage)
	
} // end namespace
} // end namespace

#endif // __VERTEX_BUFFER_STORAGE_H_B105D5EA_0949_4365_A66A_5DD945D5B951_
