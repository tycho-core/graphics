//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 4 April 2008 6:29:14 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SHAPES_H_1AE8A82E_6E92_4804_948A_0A19093F085F_
#define __SHAPES_H_1AE8A82E_6E92_4804_948A_0A19093F085F_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/renderer.h"
#include "graphics/index_buffer.h"
#include "graphics/mesh.h"
#include "math/vector2.h"
#include "math/rect.h"
#include "math/matrix4x4.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	enum shape_create_flags
	{
		shape_create_default = 0,
		shape_create_normals = (1 << 0), ///< calculate normals for the vertices when generating the shape
		shape_create_uvs     = (1 << 1)  ///< setup default texture coordinates for the shape
	};

	/// creates an axis aligned box. This will create vertex and index buffers and return a graphics::mesh for the object
	TYCHO_GRAPHICS_ABI graphics::mesh_ptr create_box(graphics::renderer_ptr ri, const math::vector3f &size, int create_flags);
	
	/// creates a plane orientated in the x/y plane. This will create vertex and index buffers and return a graphics::mesh for the object
	TYCHO_GRAPHICS_ABI graphics::mesh_ptr create_plane(graphics::renderer_ptr ri, const math::vector2f &size, int create_flags);

	/// creates a plane orientated in the x/y plane. This will create vertex and index buffers and return a graphics::mesh for the object
	TYCHO_GRAPHICS_ABI graphics::mesh_ptr create_plane(graphics::renderer_ptr ri, 
											   const math::vector2f &size, 
											   int create_flags, 
											   const math::matrix4x4f&,
											   const math::rectf& uvs);

 	TYCHO_GRAPHICS_ABI int create_box(const math::vector3f &size, const vertex_format &format, void *vb_ptr, index_buffer::format ib_format, void *ib_ptr, int create_flags);

 	TYCHO_GRAPHICS_ABI int create_plane(const math::vector2f &size, 
 								const vertex_format &format, 
 								void *vb_ptr, 
 								index_buffer::format ib_format, 
 								void *ib_ptr, 
 								int ib_offset, 
 								int create_flags, 
 								const math::matrix4x4f&,
 								const math::rectf& uvs);
 								
	TYCHO_GRAPHICS_ABI int create_sphere(vertex_buffer *, index_buffer *, float size);

} // end namespace
} // end namespace

#endif // __SHAPES_H_1AE8A82E_6E92_4804_948A_0A19093F085F_
