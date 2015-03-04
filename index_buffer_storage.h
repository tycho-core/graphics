//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 4 April 2008 1:00:19 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __INDEX_BUFFER_STORAGE_H_F88DBF23_FC40_46C1_AB2C_F6947486A347_
#define __INDEX_BUFFER_STORAGE_H_F88DBF23_FC40_46C1_AB2C_F6947486A347_

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

	/// abstraction of index buffer storage. this could be in GPU memory or in a system
	/// memory buffer.
    class TYCHO_GRAPHICS_ABI index_buffer_storage : public graphics::buffer_storage
    {
    public:   
		index_buffer_storage(int num_bytes, int create_flags);
		
    protected:
        int get_create_flags() const;

    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, index_buffer_storage)
		int m_create_flags;
    };
    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, index_buffer_storage)

} // end namespace
} // end namespace

#endif // __INDEX_BUFFER_STORAGE_H_F88DBF23_FC40_46C1_AB2C_F6947486A347_
