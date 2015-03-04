//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 4 April 2008 6:29:14 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "shapes.h"
#include "graphics/vertex_format.h"
#include "graphics/index_buffer.h"
#include "graphics/vertex_buffer.h"
#include "graphics/renderer.h"
#include "graphics/mesh.h"
#include "graphics/vertex_formats.h"
#include "math/vector3.h"
#include "math/rect.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
using namespace tycho::core;
using namespace tycho::math;

namespace tycho
{
namespace graphics
{

	static void create_box_fill_cb(graphics::renderer_ptr ri, 
								   graphics::mesh_ptr mesh, 
								   void* vb_ptr, int /*vb_size*/, void* ib_ptr,								   
								   const math::vector3f &size, 
								   int create_flags)
	{
		graphics::create_box(size, *mesh->m_vb->get_format(), vb_ptr, index_buffer::f_int16, ib_ptr, create_flags);
	}
	
	TYCHO_GRAPHICS_ABI graphics::mesh_ptr create_box(graphics::renderer_ptr ri, const math::vector3f &size, int create_flags)
	{
		using namespace graphics;			
        using namespace std::placeholders;


		// make a nice box, a square box, a beautiful box.
		const vertex_format* vtx_fmt = &vertex_formats::pos3f32_nml3f32;
		int num_verts = graphics::create_box(size, *vtx_fmt, 0, index_buffer::f_int16, 0, create_flags);
		
		return ri->create_mesh((vertex_format*)vtx_fmt, num_verts, vertex_buffer::cf_static, 
					    index_buffer::f_int16, 36, index_buffer::cf_static,
					    std::bind(create_box_fill_cb, ri, _1, _2, _3, _4, size, create_flags));
	}

	static void create_plane_fill_cb(graphics::mesh_ptr mesh, void* vb_ptr, void* ib_ptr,
									 const math::vector2f &size, int create_flags, math::matrix4x4f mat, math::rectf uvs)
	{
		graphics::create_plane(size, *mesh->m_vb->get_format(), vb_ptr, index_buffer::f_int16, ib_ptr, 0, create_flags, mat, uvs);
	}
	
	TYCHO_GRAPHICS_ABI graphics::mesh_ptr create_plane(graphics::renderer_ptr ri, const math::vector2f &size, int create_flags)
	{
		return create_plane(ri, size, create_flags, math::matrix4x4f::create_identity(), math::rectf(0, 0, 1, 1));
	}	

	TYCHO_GRAPHICS_ABI graphics::mesh_ptr create_plane(graphics::renderer_ptr ri, 
											   const math::vector2f &size, 
											   int create_flags, 
											   const math::matrix4x4f& matrix, 
											   const math::rectf& uvs)
	{
		using namespace graphics;
        using namespace std::placeholders;

		const vertex_format* vtx_fmt = &vertex_formats::pos3f32_nml3f32_uv2f32;
		int num_verts = graphics::create_plane(size, *vtx_fmt, 0, index_buffer::f_int16, 0, 0, create_flags, matrix, uvs);		
		return ri->create_mesh((vertex_format*)vtx_fmt, num_verts, vertex_buffer::cf_static,
							   index_buffer::f_int16, 6, index_buffer::cf_static,
							   std::bind(&create_plane_fill_cb, _1, _2, _4, size, create_flags, matrix, uvs));							   	
	}

	TYCHO_GRAPHICS_ABI int create_box(const math::vector3f& size, 
							  const vertex_format& format, 
							  void *vb_ptr, 
							  index_buffer::format ib_format, 
							  void *ib_ptr,
							  int create_flags)
	{
		// create a simple box
		float hx = size.x() / 2;
		float hy = size.y() / 2;
		float hz = size.z() / 2;
		
		math::vector3f vertices[8] = {
			// back quadrant
			math::vector3f(-hx,  hy, -hz), // tl
			math::vector3f( hx,  hy, -hz), // tr
			math::vector3f( hx, -hy, -hz), // br
			math::vector3f(-hx, -hy, -hz), // bl

			// front quadrant
			math::vector3f(-hx,  hy, hz), // tl
			math::vector3f( hx,  hy, hz), // tr
			math::vector3f( hx, -hy, hz), // br
			math::vector3f(-hx, -hy, hz), // bl
		};
					
		// index as a triangle list, vertices are ordered clockwise.
		core::int16 indices[36] = {
			// back face
			0, 1, 2,
			2, 3, 0,
			
			// front face
			5, 4, 7,
			7, 6, 5,
			
			// left face
			0, 3, 7,
			7, 4, 0,
			
			// right face
			1, 5, 6,
			6, 2, 1,
			
			// top face
			0, 4, 5,
			5, 1, 0,
			
			// bottom face
			3, 2, 0,
			0, 7, 3
		};
		
		
		
		// create our vertex and normal arrays, need to expand vertices
		struct vertex 
		{
 			math::vector3f v;
 			math::vector3f n;
 		};
 		
 		
		math::vector3f btl = math::vector3f(-hx,  hy, hz); // tl
		math::vector3f btr = math::vector3f( hx,  hy, hz); // tr
		math::vector3f bbr = math::vector3f( hx, -hy, hz); // br
		math::vector3f bbl = math::vector3f(-hx, -hy, hz); // bl

		// front quadrant
		math::vector3f ftl = math::vector3f(-hx,  hy, -hz); // tl
		math::vector3f ftr = math::vector3f( hx,  hy, -hz); // tr
		math::vector3f fbr = math::vector3f( hx, -hy, -hz); // br
		math::vector3f fbl = math::vector3f(-hx, -hy, -hz); // bl

 		vertex vertices_nml[24] = {
 			// back face
			{ btr, math::vector3f(0, 0, 1)},
			{ btl, math::vector3f(0, 0, 1)},
			{ bbl, math::vector3f(0, 0, 1)},
			{ bbr, math::vector3f(0, 0, 1)},
 			
 			// front face
			{ ftl, math::vector3f(0, 0, -1)},
			{ ftr, math::vector3f(0, 0, -1)},
			{ fbr, math::vector3f(0, 0, -1)},
			{ fbl, math::vector3f(0, 0, -1)},
			
 			// left face
			{ btl, math::vector3f(-1, 0, 0)},
			{ ftl, math::vector3f(-1, 0, 0)},
			{ fbl, math::vector3f(-1, 0, 0)},
			{ bbl, math::vector3f(-1, 0, 0)},
 			
 			// right face
			{ ftr, math::vector3f(1, 0, 0)},
			{ btr, math::vector3f(1, 0, 0)},
			{ bbr, math::vector3f(1, 0, 0)},
			{ fbr, math::vector3f(1, 0, 0)},
 			
 			// top face
			{ btl, math::vector3f(0, 1, 0)},
			{ btr, math::vector3f(0, 1, 0)},
			{ ftr, math::vector3f(0, 1, 0)},
			{ ftl, math::vector3f(0, 1, 0)},
 			
 			// bottom face
			{ fbl, math::vector3f(0, -1, 0)},
			{ fbr, math::vector3f(0, -1, 0)},
			{ bbr, math::vector3f(0, -1, 0)},
			{ bbl, math::vector3f(0, -1, 0)} 			
 		};
 		
 		core::uint16 indices_nml[36] = {
			// back face
			0, 1, 2,
			2, 3, 0,
			
			// front face
			4, 5, 6,
			6, 7, 4,
			
			// left face
			8, 9, 10,
			10, 11, 8,
			
			// right face
			12, 13, 14, 
			14, 15, 12,
			
			// top face
			16, 17, 18,
			18, 19, 16,
			
			// bottom face
			20, 21, 22, 
			22, 23, 20
 		};
 		
 		
		// copy the vertices into the vertex buffer. we do this by creating a vertex_format
		// describing our data and copying it into the vb so any conversion can take place
		// for the underlying format.
		int num_verts = 0;
		if(create_flags & shape_create_normals)
			num_verts = 24;
		else
			num_verts = 8;
		if(vb_ptr)
		{
			if(create_flags & shape_create_normals)
			{
 				vertex_format our_format;
 				our_format.add_component(vertex_format::make_component_decl(vertex_format::ct_float, 3));
 				our_format.add_component(vertex_format::make_component_decl(vertex_format::ct_float, 3));
 				our_format.add_semantic(vertex_format::make_semantic_decl(vertex_format::s_position, 0, 0, vertex_format::c_xyz));
 				our_format.add_semantic(vertex_format::make_semantic_decl(vertex_format::s_normal, 0, 1, vertex_format::c_xyz));
 				vertex_buffer::write(our_format, vertices_nml, format, vb_ptr, num_verts);
			}
			else
			{
 				vertex_format our_format;
 				our_format.add_component(vertex_format::make_component_decl(vertex_format::ct_float, 3));
 				our_format.add_semantic(vertex_format::make_semantic_decl(vertex_format::s_position, 0, 0, vertex_format::c_xyz));
 				vertex_buffer::write(our_format, vertices, format, vb_ptr, num_verts);			
			}
		}
				
		// add indices to the index buffer
		if(ib_ptr)
		{
			if(create_flags & shape_create_normals)
				index_buffer::write(index_buffer::f_int16, indices_nml, ib_format, ib_ptr, 36);
			else
				index_buffer::write(index_buffer::f_int16, indices, ib_format, ib_ptr, 36);
		}
		return num_verts;
		
	}
	
	TYCHO_GRAPHICS_ABI int create_plane(const math::vector2f& size, 
							  const vertex_format& format, 
							  void *vb_ptr, 
							  index_buffer::format ib_format, 
							  void *ib_ptr,
							  int ib_offset,
							  int /*create_flags*/,
							  const math::matrix4x4f& trans,
							  const math::rectf& uvs)
	{
		using namespace math;
		
		struct vertex
		{
			vector3f pos;
			vector3f nml;
			vector2f uv;
		};
		
		float hx = size.x() / 2.0f;
		float hy = size.y() / 2.0f;
		vertex vertices[4] = {
			{ vector3f(-hx, -hy, 0), vector3f(0,0,-1), uvs.get_bottom_left() },
			{ vector3f(-hx,  hy, 0), vector3f(0,0,-1), uvs.get_top_left()},
			{ vector3f( hx,  hy, 0), vector3f(0,0,-1), uvs.get_top_right() },
			{ vector3f( hx, -hy, 0), vector3f(0,0,-1), uvs.get_bottom_right() }
		};
		if(vb_ptr)
		{
			// transform form them with user transform
			for(int i = 0; i < 4; ++i)
				vertices[i].pos = trans * vertices[i].pos;
			vertex_buffer::write(vertex_formats::pos3f32_nml3f32_uv2f32, vertices, format, vb_ptr, 4);
		}

		if(ib_ptr)
		{
			// 1 - 2
			// | / |
			// |/  |
			// 0 - 3
			core::int16 indices[6] = {
				0, 1, 2,
				2, 3, 0
			};			
			for(int i = 0; i < 6; ++i)
				indices[i] = static_cast<core::int16>(indices[i] + ib_offset);
			index_buffer::write(index_buffer::f_int16, indices, ib_format, ib_ptr, 6);
		}
				
		return 4;
	}
		
	TYCHO_GRAPHICS_ABI int create_sphere(vertex_buffer *, index_buffer *, float)
	{
		return 0;	
	}
	
	TYCHO_GRAPHICS_ABI int create_plane(vertex_buffer *, index_buffer *, float, float)
	{
		return 0;		
	}

} // end namespace
} // end namespace
