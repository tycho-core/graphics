//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 5 April 2008 1:12:50 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __MESH_H_ADC4D2D6_50BD_40D6_95F5_B2F8A0A67443_
#define __MESH_H_ADC4D2D6_50BD_40D6_95F5_B2F8A0A67443_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/resource.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// a mesh is a unit of rendering geometry, it contains all the geometry
	/// for the mesh.
    class TYCHO_GRAPHICS_ABI mesh : public graphics::resource
    {
    public:
		vertex_buffer_ptr m_vb;
		index_buffer_ptr  m_ib;

	private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, mesh);
    };

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, mesh)

} // end namespace
} // end namespace

#endif // __MESH_H_ADC4D2D6_50BD_40D6_95F5_B2F8A0A67443_
