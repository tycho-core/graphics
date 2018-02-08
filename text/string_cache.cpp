//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 30 May 2008 1:00:48 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "string_cache.h"
#include "graphics/renderer.h"
#include "graphics/format.h"
#include "graphics/texture.h"
#include "graphics/camera.h"
#include "graphics/render_op.h"
#include "graphics/render_target_texture.h"
#include "graphics/vertex_formats.h"
#include "graphics/shader/parameter_buffer_view.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace text
{

	string_cache::string_cache(graphics::renderer* r, int width, int height) :
		m_renderer(r),
		m_allocator(width, height)
	{
		m_texture = r->create_render_texture(format::pixel_argb8888, width, height);
		m_shader  = r->create_system_shader(graphics::system_shader_ui);
	}
	
	string_cache::~string_cache()
	{
		for(size_t i = 0; i < m_batches.size(); ++i)
		{
			for(size_t j = 0; j < m_batches[i].m_batches.size(); ++j)
				delete m_batches[i].m_batches[j];
		}
		m_batches.clear();
		m_texture = graphics::render_target_texture_ptr();
	}
		
	int string_cache::get_max_width() const
	{
		return m_allocator.get_max_width();
	}
	
	int string_cache::get_max_height() const
	{
		return m_allocator.get_max_height();
	}
	
	string_cache::handle string_cache::allocate_space(int width, int height)
	{
		using namespace math;
		core::allocator_2d::tag* t = m_allocator.allocate(width, height);
		if(t)
		{
			handle_* h = new handle_;		
			h->tag = t;
			h->verts.set(0, vector2i(t->x, t->y));						   // tl
			h->verts.set(1, vector2i(t->x + t->width, t->y));			   // tr
			h->verts.set(2, vector2i(t->x + t->width, t->y + t->height));  // br
			h->verts.set(3, vector2i(t->x, t->y + t->height));			   // bl
			float max_width = (float)m_allocator.get_max_width();
			float max_height = (float)m_allocator.get_max_height();
			h->uvs = math::rectf(t->x / max_width, t->y / max_height, t->width / max_width, t->height / max_height);
			h->texture = m_texture;				
			return h;
		}
		
		return 0;
	}
	
	void string_cache::free(handle h_)
	{
		handle_* h = make_handle(h_);
		m_allocator.free(reinterpret_cast<core::allocator_2d::tag*>(h->tag));
		delete h;
	}
	
	string_cache::glyph* string_cache::allocate_glyph_array(int n)
	{
		return new glyph[n];
	}
	
	void string_cache::free_glyph_array(glyph* g)
	{
		delete[] g;
	}
		
	void string_cache::render_string(handle h_, glyph* glyphs, int num_glyphs, std::vector<graphics::texture_ptr>& textures)
	{		
		handle_* h = make_handle(h_);
		
		// separate glyphs into render batches depending on their source and destination textures.
		for(int i = 0; i < num_glyphs; ++i)
		{
			glyph& cur = glyphs[i];
			batch* b = find_batch(h->texture, textures[cur.texture]);
			TYCHO_ASSERT(b);
			cur.dstx += h->tag->x;
			cur.dsty += h->tag->y;
			b->glyphs.push_back(cur);			
		}
		free_glyph_array(glyphs);
	}

	string_cache::batch* string_cache::find_batch(graphics::render_target_texture_ptr dst_texture, graphics::texture_ptr src_texture)
	{
		for(size_t i = 0; i < m_batches.size(); ++i)
		{
			if(m_batches[i].m_texture == dst_texture)
			{
				std::vector<batch*>& batch = m_batches[i].m_batches;
				for(size_t j = 0; j < batch.size(); ++j)
				{
					if(batch[j]->src_texture == src_texture)
					{
						return batch[j];
					}
				}
				
				// no batch for src texture so make one
				string_cache::batch* new_batch = new string_cache::batch();
				new_batch->src_texture = src_texture;
				m_batches[i].m_batches.push_back(new_batch);
				return new_batch;				
			}
		}
		
		// no batch for the destination texture
		string_cache::texture new_texture;
		new_texture.m_texture = dst_texture;
		m_batches.push_back(new_texture);
		return find_batch(dst_texture, src_texture);
	}
	
	const math::rectf& string_cache::get_string_uvs(handle h_) const
	{
		const handle_* h = make_handle(h_);
		return h->uvs;
	}	
	
	math::vector2i string_cache::get_string_size(handle h_) const
	{
		const handle_* h = make_handle(h_);	
		return math::vector2i(h->tag->width, h->tag->height);
	}
	
	graphics::texture_base_ptr string_cache::get_texture(handle)
	{
		return m_texture;
	}
	
	void string_cache::render_batches()
	{
		using namespace math;
		using namespace std::placeholders;

		// render all batches
		for(size_t i = 0; i < m_batches.size(); ++i)
		{
			texture& tex = m_batches[i];
			
			// setup render target
			graphics::camera_ptr camera = graphics::camera_ptr(new graphics::camera());
			matrix4x4f view_mat = matrix4x4f::create_identity();
			matrix4x4f proj_mat(math::make_perspective(1.0f, 1.0f, 1.0f, 50000.0f));	
			matrix4x4f view_proj_mat = view_mat * proj_mat;	
			m_renderer->set_render_target(tex.m_texture);
			m_renderer->set_viewport(viewport(vector2f(0,0), vector2f(1,1), 0, 1));
			
			for(size_t j = 0; j < tex.m_batches.size(); ++j)
			{		
				batch* b = tex.m_batches[j];
				int num_glyphs = static_cast<int>(b->glyphs.size());
				int num_verts = num_glyphs * 4;
				int num_indices = num_glyphs * 6;
				
				render_op_ptr op = m_renderer->create_render_op();
				
				// setup the shader params for this call
				shader::parameter_buffer_view view(m_shader->create_default_table(), m_shader->get_parameter_layout());
				view.set("diffuse_texture", b->src_texture.get());
				view.set("view_proj", view_proj_mat);
				view.set("world_matrix", matrix4x4f::create_identity());
				
				// grab some dynamic VB space to store the vertices 				
				vertex_format* vtx_fmt = &vertex_formats::pos3f32_nml3f32_uv2f32;
				vertex_buffer_ptr vb = m_renderer->frame_allocate_vertex_buffer(vtx_fmt, num_verts);
				m_renderer->update_vertex_buffer(vb, 0, vtx_fmt->size() * num_verts, 
					std::bind(&string_cache::batch_fill_vb, this, b, _2, _3));
									
				// use the builtin quad index list to render it with
				op->vertex_buffers[0] = vb;
				op->index_buffers[0]  = m_renderer->get_quad_index_buffer(num_glyphs);				
				op->first_vertex = 0;				
				op->num_vertices = num_verts;
				op->num_indices = num_indices;
 				op->shader = m_shader;
 				op->shader_params = view.get_buffer();
				op->primitive = primitive_type_tri_list;
				
				// submit render op
				m_renderer->render(op);
			}
		}	
		// all done
		m_batches.clear();
	}
	
	void string_cache::batch_fill_vb(batch* b, void* vb_ptr, int vb_size)
	{
		TYCHO_ASSERT(b);
		TYCHO_ASSERT(vb_ptr);
		TYCHO_ASSERT(vb_size);

		using namespace math;
		
		struct vert
		{
			vector3f pos;
			vector3f nml;			
			vector2f uv;
		} *verts = (vert*)vb_ptr;
		 
		float dw = (float)m_texture->get_width();
		float dh = (float)m_texture->get_height();
		float sw = (float)b->src_texture->get_width();
		float sh = (float)b->src_texture->get_height();
				
		//TODO: need to get the texel offset from the renderer as it is dependent on that.
		float dxhalf = 1 / (2* dw);
		vector3f half_texel(dxhalf, dxhalf, 0);
		for(size_t i = 0; i < b->glyphs.size(); ++i)
		{
			glyph& g = b->glyphs[i];			
			float dstw = g.width / dw;
			float dsth = g.height / dh;
			float srcw = g.width / sw;
			float srch = g.height / sh;
			float srcx = g.srcx / sw;
			float srcy = g.srcy / sh;
			float dstx = g.dstx / dw;
			float dsty = g.dsty / dh;


			// bl
			verts->nml = vector3f(0,0,-1);
			verts->pos = vector3f(dstx - dxhalf, 1 - (dsty + dsth) + dxhalf, 0); 
			verts->uv  = vector2f(srcx, srcy + srch);
			++verts;

			// tl 
			verts->nml = vector3f(0,0,-1);
			verts->pos = vector3f(dstx - dxhalf, 1 - dsty + dxhalf, 0); 
			verts->uv  = vector2f(srcx, srcy);
			++verts;

			// tr 
			verts->nml = vector3f(0,0,-1);
			verts->pos = vector3f(dstx + dstw - dxhalf, 1 - dsty + dxhalf, 0); 
			verts->uv  = vector2f(srcx + srcw, srcy);
			++verts;

			// br 
			verts->nml = vector3f(0,0,-1);
			verts->pos = vector3f(dstx + dstw - dxhalf, 1 - (dsty + dsth) + dxhalf, 0) - half_texel; 
			verts->uv  = vector2f(srcx + srcw, srcy + srch);
			++verts;
		}
		
		delete b;
	}

} // end namespace
} // end namespace
} // end namespace
