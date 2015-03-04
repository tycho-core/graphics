//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:57:46 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/renderer.h"
#include "graphics/cube_texture.h"
#include "graphics/window.h"
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"
#include "graphics/vertex_buffer_storage.h"
#include "graphics/index_buffer_storage.h"
#include "graphics/texture.h"
#include "graphics/texture_storage.h"
#include "graphics/render_target_texture.h"
#include "graphics/mesh.h"
#include "graphics/render_op.h"
#include "graphics/gpu_events.h"
#include "graphics/shader/gamecube_shader.h"
#include "graphics/text/font_manager.h"
#include "graphics/shader/parameter_buffer_view.h"
#include "image/image.h"
#include "core/memory.h"
#include <map>
#include <functional>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
    const int renderer::NumQuads  = 2048;
    
    namespace detail
    {
	struct locked_texture
	{
		bool operator<(const locked_texture& rhs)
		{
			return texture < rhs.texture;
		}
		
		texture_ptr texture;			
		char*		out_buffer;
		int			out_stride;
	};			
    }

	
	/// constructor
	renderer::renderer(io::interface* io, render_interface *i) :
        m_io(io),
		m_interface(i),
		m_font_manager(0),
		m_cur_render_frame(0),
		m_cur_update_frame(0)
	{
		core::mem_zero(m_frame_data);
	}
		
	/// destructor
	renderer::~renderer()
	{
		m_cur_update_frame = 0;
		m_cur_render_frame = 0;		
	}

	//////////////////////////////////////////////////////////////////////////
	// Device enumeration
	//////////////////////////////////////////////////////////////////////////
	
	/// query the number of adapters available
	int renderer::get_num_adapters()
	{
		return m_interface->get_num_adapters();
	}
	
	/// \returns detailed information about the i'th adapter
	const adapter* renderer::get_adapter(int adapter_idx)
	{
		return m_interface->get_adapter(adapter_idx);
	}
	
	
	/// fills an index buffer 
	static void quad_index_writer(int num_quads, void* ib_ptr, int ib_size)
	{
		using namespace core;
		TYCHO_ASSERT(ib_size == (int)(num_quads * 6 * sizeof(int16)));
		int16* ib = (int16*)ib_ptr;
		int16 v = 0;
		for(int i = 0; i < num_quads; ++i, v += 4)
		{
			*ib++ = v + 0;
			*ib++ = v + 1;
			*ib++ = v + 2;
			*ib++ = v + 2;
			*ib++ = v + 3;
			*ib++ = v + 0;			
		}
	}
	
	/// initialise the renderer on the specified adapter
	bool renderer::initialise(int adapter_idx)
	{
        using namespace std::placeholders;

		bool res = m_interface->initialise(adapter_idx);
		if(res)
		{
			// per frame render data 
			for(int i = 0; i < NumFrames; ++i)
				m_frame_data[i] = new renderer_frame_data(this);	
			m_cur_update_frame = m_frame_data[0];
			m_cur_render_frame = m_frame_data[1];
			for(int i = 0; i < NumFrames; ++i)
				m_frame_data[i]->initialise();
			
			// configure string renderer
			m_font_manager = new text::font_manager(m_io, this);
			if(!m_font_manager)
			{
				res = false;
			}
			
			// create index list for rendering quads
			m_quad_index_list = create_index_buffer(index_buffer::f_int16, renderer::NumQuads * 6, 0,
                                                    std::bind(&quad_index_writer, renderer::NumQuads, _2, _3));
		}
			
			
		return res;
	}
	
	/// release the interface
	bool renderer::destroy()
	{
		m_quad_index_list = index_buffer_ptr();
		for(int i = 0; i < NumFrames; ++i)
			delete m_frame_data[i];
		core::mem_zero(m_frame_data);
		core::safe_delete(m_font_manager);
		core::safe_delete(m_interface);
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	// Graphics object creation
	//////////////////////////////////////////////////////////////////////////

	window_ptr renderer::create_window(int width, 
					   int height, 
					   graphics::format pixel_fmt, 
					   graphics::format depth_fmt,
					   void *platform_specific)
	{	
		window_ptr new_window(new window(width, height));
		new_window->set_storage(m_interface->create_window(width, height, pixel_fmt, depth_fmt, platform_specific));
		return new_window;
	}

	
	vertex_buffer_ptr renderer::create_vertex_buffer(vertex_format *fmt, 
		int num_verts, int create_flags,
		vertex_buffer_storage_fill_func fill_func)
	{		
		int num_bytes = num_verts * fmt->size();
		vertex_buffer_ptr vb(new vertex_buffer(fmt, num_verts));
		vertex_buffer_storage_ptr storage(new vertex_buffer_storage(num_bytes, create_flags));
		vb->set_storage(storage);
		renderer_frame_data::create_vertex_buffer_storage cb;		
		cb.create_flags = create_flags;
		cb.num_bytes = num_bytes;
		cb.store = storage;
		cb.type = renderer_frame_data::resource_type_static;
		cb.user_fn = fill_func;
		m_cur_update_frame->vb_creates.push_back(cb);
		return vb;
	}
			
	vertex_buffer_ptr renderer::create_dynamic_vertex_buffer(vertex_buffer_storage_ptr,
										   vertex_format* fmt, 
										   int offset,
										   int num_verts)
	{
		//TODO 
		return vertex_buffer_ptr();
	}
		
	vertex_buffer_storage_ptr renderer::create_vertex_buffer_storage(int num_bytes, int create_flags)
	{
		vertex_buffer_storage_ptr storage(new vertex_buffer_storage(num_bytes, create_flags));
		renderer_frame_data::create_vertex_buffer_storage cb;		
		cb.create_flags = create_flags;
		cb.num_bytes = num_bytes;
		cb.store = storage;
		cb.type = renderer_frame_data::resource_type_static;
		m_cur_update_frame->vb_creates.push_back(cb);
		return storage;
	}
	
	vertex_buffer_ptr renderer::create_vertex_buffer(vertex_buffer_storage_ptr storage, vertex_format* fmt, int offset, int num_verts)
	{
		vertex_buffer_ptr vb(new vertex_buffer(fmt, num_verts));
		vb->set_storage(storage);
		return vb;	
	}
	
	index_buffer_ptr renderer::create_index_buffer(index_buffer::format type, 
		int num_indices, int create_flags)
	{
		return create_index_buffer(type, num_indices, create_flags, index_buffer_fill_func());
	}
	
	index_buffer_ptr renderer::create_index_buffer(index_buffer::format type, 
		int num_indices, int create_flags, 
		index_buffer_fill_func fill_func)
	{
		int num_bytes = index_buffer::get_num_bytes(type, num_indices);
		index_buffer_ptr ib(new index_buffer(type, num_indices));
		index_buffer_storage_ptr storage(new index_buffer_storage(num_bytes, create_flags));
		ib->set_storage(storage);
		renderer_frame_data::create_index_buffer_storage cb;		
		cb.create_flags = create_flags;
		cb.num_bytes = num_bytes;
		cb.num_indices = num_indices;
		cb.store = storage;
		cb.type = renderer_frame_data::resource_type_static;
		cb.format = type;		
		cb.user_fn = fill_func;
		m_cur_update_frame->ib_creates.push_back(cb);
		return ib;
	}

	mesh_ptr renderer::create_mesh(vertex_format *vb_fmt, int vb_num_verts, int vb_create_flags, 
						index_buffer::format ib_type, int ib_num_indices, int ib_create_flags, 
						mesh_fill_func fill_func)
	{
		int ib_bytes = index_buffer::get_num_bytes(ib_type, ib_num_indices);
		int vb_bytes = vb_num_verts * vb_fmt->size();
		vertex_buffer_ptr vb(new vertex_buffer(vb_fmt, vb_num_verts));
		index_buffer_ptr ib(new index_buffer(ib_type, ib_num_indices));
		vertex_buffer_storage_ptr vbs(new vertex_buffer_storage(vb_bytes, vb_create_flags));
		index_buffer_storage_ptr ibs(new index_buffer_storage(ib_bytes, ib_create_flags));
		vb->set_storage(vbs);
		ib->set_storage(ibs);
		mesh_ptr new_mesh = mesh_ptr(new mesh);
		new_mesh->m_vb = vb;
		new_mesh->m_ib = ib;
		renderer_frame_data::create_mesh cb;
		cb.cvb.create_flags = vb_create_flags;
		cb.cvb.num_bytes = vb_bytes;
		cb.cvb.store = vbs;
		cb.cvb.type = renderer_frame_data::resource_type_static;
		cb.cib.create_flags = ib_create_flags;
		cb.cib.num_bytes = ib_bytes;
		cb.cib.num_indices = ib_num_indices;
		cb.cib.format = ib_type;
		cb.cib.store = ibs;
		cb.cib.type = renderer_frame_data::resource_type_static;
		cb.user_fn = fill_func;
		cb.mesh = new_mesh;
		m_cur_update_frame->mesh_creates.push_back(cb);
		return new_mesh;	
	}
		
	texture_ptr renderer::create_texture(tycho::image::image_base_ptr img)
	{
		texture_ptr tex(new texture(this, img->get_width(), img->get_height(), img->get_num_mips()));
		renderer_frame_data::create_texture cb;
		cb.texture = tex;
		cb.image = img;
		cb.type = renderer_frame_data::resource_type_static;		
		m_cur_update_frame->texture_creates.push_back(cb);
		return tex;
	}

	texture_ptr renderer::create_texture(format format, 
		int width, int height, int num_mips, 
		texture_fill_func fill_func)
	{
		texture_ptr tex(new texture(this, width, height, num_mips));		
		renderer_frame_data::create_texture cb;
		cb.texture = tex;
		cb.texture_format = format;
		cb.width = width;
		cb.height = height;
		cb.num_mips = num_mips;
		cb.user_fn = fill_func;
		cb.type = renderer_frame_data::resource_type_static;
		m_cur_update_frame->texture_creates.push_back(cb);
		return tex;
	}
	
	texture_ptr renderer::create_dynamic_texture(format format, int width, int height)
	{
		texture_ptr tex(new texture(this, width, height, 1));
		renderer_frame_data::create_texture cb;
		cb.texture = tex;
		cb.texture_format = format;
		cb.width = width;
		cb.height = height;
		cb.num_mips = 1;
		cb.type = renderer_frame_data::resource_type_dynamic;
		m_cur_update_frame->texture_creates.push_back(cb);		
		return tex;	
	}
	
	render_target_texture_ptr renderer::create_render_texture(format format, int width, int height)
	{
		render_target_texture_ptr texture(new render_target_texture(this, width, height));
		renderer_frame_data::create_render_texture cb;
		cb.texture_format = format;
		cb.width = width;
		cb.height = height;
		cb.texture = texture;
		m_cur_update_frame->rtt_creates.push_back(cb);
		return texture;	
	}
	
	
	render_op_ptr renderer::create_render_op()
	{
		return render_op_ptr(m_interface->create_render_op());
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Object updating
	//////////////////////////////////////////////////////////////////////////

	void renderer::update_texture(texture_ptr texture, int x, int y, int width, int height, texture_fill_func fill_func)
	{
		renderer_frame_data::update_texture update;
		update.texture = texture;
		update.dst_x = x;
		update.dst_y = y;
		update.width = width;
		update.height = height;
		update.user_fn = fill_func;
		m_cur_update_frame->texture_updates.push_back(update);
	}
		
		
	void renderer::update_vertex_buffer(vertex_buffer_ptr vb, int offset, int num_bytes, vertex_buffer_fill_func func)
	{
		renderer_frame_data::update_vertex_buffer uvb;
		uvb.vb = vb;
		uvb.offset = offset;
		uvb.num_bytes = num_bytes;
		uvb.user_fn = func;
		m_cur_update_frame->vb_updates.push_back(uvb);
	}
	
	void renderer::update_index_buffer(index_buffer_ptr ib, int offset, int num_bytes, index_buffer_fill_func func)
	{
		renderer_frame_data::update_index_buffer uib;
		uib.ib = ib;
		uib.offset = offset;
		uib.num_bytes = num_bytes;
		uib.user_fn = func;
		m_cur_update_frame->ib_updates.push_back(uib);	
	}
		
	//////////////////////////////////////////////////////////////////////////
	// Shaders 
	//////////////////////////////////////////////////////////////////////////

	/// create a new system shader
	shader::shader_base_ptr	renderer::create_system_shader(system_shader_type t)
	{	
		return m_interface->create_system_shader(t);
	}	

	/// create a new gamecube shader
	shader::gamecube_shader_ptr renderer::create_gamecube_shader()
	{
		return shader::gamecube_shader_ptr(m_interface->create_gamecube_shader());
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	// Scene rendering
	//////////////////////////////////////////////////////////////////////////
	
	/// must be called before rendering to the passed render target
	void renderer::set_render_target(render_target_ptr rt)
	{
		// add a new render target to the frame date and set 
		m_cur_update_frame->render_targets.push_back(renderer_frame_data::render_target_data(rt));
	}
	
	/// set the viewport 
	void renderer::set_viewport(const graphics::viewport& vp)
	{
		TYCHO_ASSERT(m_cur_update_frame->render_targets.size() > 0);
		m_cur_update_frame->render_targets.back().viewports.push_back(renderer_frame_data::render_target_data::viewport_data(vp));
	}
	
	/// submit a render operation to the driver
	void renderer::render(render_op_ptr op)
	{
		TYCHO_ASSERT(m_cur_update_frame->render_targets.size() > 0);
		TYCHO_ASSERT(m_cur_update_frame->render_targets.back().viewports.size() > 0);
		m_cur_update_frame->render_targets.back().viewports.back().ops.push_back(op);
	}

	void renderer::present(render_target_ptr rt)
	{
		m_cur_update_frame->present_render_targets.push_back(rt);		
	}

	//////////////////////////////////////////////////////////////////////////
	// Frame management
	//////////////////////////////////////////////////////////////////////////
	
	/// flush all queued data to the GPU.
	bool renderer::flush()
	{
		return false;
//		return m_interface->flush();
	}

	/// flip to front buffer
	bool renderer::flip()
	{
		// \todo stall until render thread has finished rendering its current frame
		
		// submit any text rendering we have batch, this should really happen on the render thread
		m_font_manager->render();
		
		// flip frame data buffers and let the render thread know it can start processing
		// its next frame
		std::swap(m_cur_render_frame, m_cur_update_frame);		
		rt_process_frame(m_cur_render_frame);
		return true;
	}

	/// stall until all current rendering operations are complete
	void renderer::sync()
	{
//		m_interface->sync();
	}

	//////////////////////////////////////////////////////////////////////////
	// Misc
	//////////////////////////////////////////////////////////////////////////

	/// \returns the font manager
	text::font_manager& renderer::get_font_manager()
	{
		return *m_font_manager;
	}

	/// allocate some memory for the duration of the frame
	void* renderer::frame_allocate(size_t n)
	{
		return m_cur_update_frame->allocator.allocate(n);
	}
	
	/// allocate some vb space for the duration of the frame
	vertex_buffer_ptr renderer::frame_allocate_vertex_buffer(vertex_format *fmt, int num_verts)
	{		
		int num_bytes = fmt->size() * num_verts;
		int offset = m_cur_update_frame->m_vertex_bytes_allocated;
		offset += offset % fmt->size();		
		m_cur_update_frame->m_vertex_bytes_allocated  = offset + num_bytes;
		return create_vertex_buffer(m_cur_update_frame->m_frame_vertex_buffer, fmt, offset, num_verts);
	}	
	
	/// \returns an index list for rendering quads of vertices
	index_buffer_ptr renderer::get_quad_index_buffer(int num_quads)
	{
		return m_quad_index_list;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Render thread functions
	//////////////////////////////////////////////////////////////////////////
					
	void renderer::rt_process_frame(renderer_frame_data* frame)
	{
		rt_check_thread();
		
		TYCHO_SCOPED_GPU_EVENT(this, "process_frame");
		
		// create all buffers requested this frame
		{
			//TYCHO_SCOPED_GPU_EVENT(this, "create buffers");
			
			// create all vertex buffers for this frame
			for(size_t i = 0; i < frame->vb_creates.size(); ++i)
				rt_create_vertex_buffer_storage(frame, frame->vb_creates[i]);
				
			// create all index buffers for this frame
			for(size_t i = 0; i < frame->ib_creates.size(); ++i)
				rt_create_index_buffer_storage(frame, frame->ib_creates[i]);
				
			// create all meshes for this frame
			for(size_t i = 0; i < frame->mesh_creates.size(); ++i)
			{
				renderer_frame_data::create_mesh& create = frame->mesh_creates[i];
				rt_create_vertex_buffer_storage(frame, create.cvb);
				rt_create_index_buffer_storage(frame, create.cib);
			}
		}
								
		// lock all buffers
		{
			//TYCHO_SCOPED_GPU_EVENT(this, "lock buffers");
							
			// lock all vertex buffers for update
			for(size_t i = 0; i < frame->vb_updates.size(); ++i)
				rt_update_vertex_buffer_storage(frame, frame->vb_updates[i]);
										
			// lock all index buffers for update
			for(size_t i = 0; i < frame->ib_updates.size(); ++i)
				rt_update_index_buffer_storage(frame, frame->ib_updates[i]);
		}
		
		// update buffers
		{
			//TYCHO_SCOPED_GPU_EVENT(this, "update buffers");								

			// issue callbacks to fill all vertex buffers
			rt_update_vertex_buffers(frame);
			
			// issue callbacks to fill all index buffers
			rt_update_index_buffers(frame);
			
			// issue callbacks to fill all meshes
			rt_update_meshes(frame);
		}
		
		// unlock buffers
		{		
			//TYCHO_SCOPED_GPU_EVENT(this, "unlock buffers");								
		
			// unlock vertex buffers
			for(renderer_frame_data::unique_vbs_t::iterator it(frame->unique_vbs.begin()); it != frame->unique_vbs.end(); ++it)
				it->second.store->get_driver_object()->unlock();
			
			// unlock index buffers
			for(renderer_frame_data::unique_ibs_t::iterator it(frame->unique_ibs.begin()); it != frame->unique_ibs.end(); ++it)
				it->second.store->get_driver_object()->unlock();
		}
		
		// update textures
		{
			//TYCHO_SCOPED_GPU_EVENT(this, "update textures");								

			//TODO: interleave this with vb / ib and mesh updates
			rt_update_textures(frame);
		}
		
		// preprocess all render targets
		// \todo parallelize
		for(size_t i = 0; i < frame->render_targets.size(); ++i)
		{
			rt_process_render_target(frame->render_targets[i]);
		}
		
		// wait until all updates and render target preprocess task have completed.
		
		// render all render targets
		for(size_t i = 0; i < frame->render_targets.size(); ++i)
		{
			rt_render_render_target(frame->render_targets[i]);
		}		
		
		// present to screen
		for(size_t i = 0; i < frame->present_render_targets.size(); ++i)
		{
			frame->present_render_targets[i]->get_render_target_storage()->rt_present();
		}

		// make sure to drop all references to resources used by this frame
		frame->clear();
	}
	
	void renderer::rt_update_textures(renderer_frame_data* frame)
	{	
		TYCHO_ASSERT(frame);
		rt_check_thread();

		// create all textures for this frame
		for(size_t i = 0; i < frame->texture_creates.size(); ++i)
		{
			renderer_frame_data::create_texture& create = frame->texture_creates[i];			
			texture_storage_ptr tex_store;
			if(create.image)
				tex_store = m_interface->create_texture(create.image);
			else if(create.type == renderer_frame_data::resource_type_static)
				tex_store = m_interface->create_texture(create.texture_format, create.width, create.height, create.num_mips);
			else if(create.type == renderer_frame_data::resource_type_dynamic)
				tex_store = m_interface->create_dynamic_texture(create.texture_format, create.width, create.height);
				
			TYCHO_ASSERT(tex_store);
			create.texture->set_storage(tex_store);
			create.texture->set_state(resource::s_created);
		}		

		// create all render textures
		for(size_t i = 0; i < frame->rtt_creates.size(); ++i)
		{
			renderer_frame_data::create_render_texture& create = frame->rtt_creates[i];
			render_target_texture_storage_ptr tex_store = m_interface->create_render_texture(create.texture_format, create.width, create.height);
			TYCHO_ASSERT(tex_store);
			create.texture->set_storage(tex_store);
			create.texture->set_state(resource::s_created);						
		}
		
	// update all textures for this frame		
		typedef std::map<texture_ptr, detail::locked_texture> unique_textures_t;
		
		// build a list of unique textures with updates pending and lock them as we find them		
		unique_textures_t unique_textures;		
		for(size_t i = 0; i < frame->texture_updates.size(); ++i)
		{
			renderer_frame_data::update_texture& update = frame->texture_updates[i];
			unique_textures_t::iterator it = unique_textures.find(update.texture);
			if(it == unique_textures.end())
			{
			    detail::locked_texture lt;
				lt.texture = update.texture;
				lt.texture->get_texture_storage()->rt_lock(0, &lt.out_buffer, &lt.out_stride);
				unique_textures.insert(std::make_pair(lt.texture, lt));	
			}
		}
				
		// perform copies - 
		for(size_t i = 0; i < frame->texture_updates.size(); ++i)
		{
			renderer_frame_data::update_texture& update = frame->texture_updates[i];
			unique_textures_t::iterator it = unique_textures.find(update.texture);
			TYCHO_ASSERT(it != unique_textures.end());
			detail::locked_texture& lt = it->second;
			
			// calculate offset in buffer for user to write to
			char* dst_ptr = lt.out_buffer + update.dst_y * lt.out_stride + update.dst_x;			
			update.user_fn(update.texture, dst_ptr, lt.out_stride);
			update.texture->get_texture_storage()->rt_mark_dirty(update.dst_x, update.dst_y, update.width, update.height);
		}				
		
		// unlock textures
		{
			unique_textures_t::iterator it = unique_textures.begin();
			unique_textures_t::iterator end = unique_textures.end();
			for(; it != end; ++it)
				it->second.texture->get_texture_storage()->rt_unlock(0);
		}
	}
		
	void renderer::rt_update_meshes(renderer_frame_data* frame)
	{
		TYCHO_ASSERT(frame);
		rt_check_thread();
		
		for(size_t i = 0; i < frame->mesh_creates.size(); ++i)
		{
			renderer_frame_data::create_mesh& create = frame->mesh_creates[i];
			if(create.user_fn)
			{
				void* vb_ptr = rt_get_vertex_buffer_out_buffer(frame, create.cvb);
				void* ib_ptr = rt_get_index_buffer_out_buffer(frame, create.cib);
				create.user_fn(create.mesh, vb_ptr, create.cvb.num_bytes, ib_ptr, create.cib.num_bytes);
			}
		}		
	}
	
	void* renderer::rt_get_index_buffer_out_buffer(renderer_frame_data* frame, renderer_frame_data::create_index_buffer_storage& create)	
	{
		renderer_frame_data::unique_ibs_t::iterator it = frame->unique_ibs.find(create.store->get_driver_object());
		renderer_frame_data::locked_ib& lv = it->second;
		return (char*)lv.out_buffer;// + create.store->get_buffer_offset();
	}

	void* renderer::rt_get_vertex_buffer_out_buffer(renderer_frame_data* frame, renderer_frame_data::create_vertex_buffer_storage& create)	
	{
		renderer_frame_data::unique_vbs_t::iterator it = frame->unique_vbs.find(create.store->get_driver_object());
		renderer_frame_data::locked_vb& lv = it->second;
		return (char*)lv.out_buffer;// + create.store->get_buffer_offset();	
	}
	
	void renderer::rt_update_index_buffers(renderer_frame_data* frame)
	{
		TYCHO_ASSERT(frame);
		rt_check_thread();
		
		// issue all updates
		for(size_t i = 0; i < frame->ib_creates.size(); ++i)
		{
			renderer_frame_data::create_index_buffer_storage& create = frame->ib_creates[i];
			if(create.user_fn)
			{
				void* out_buffer = rt_get_index_buffer_out_buffer(frame, create); 
				create.user_fn(create.store, out_buffer, create.num_bytes);
			}
		}

		for(size_t i = 0; i < frame->ib_updates.size(); ++i)
		{
			renderer_frame_data::update_index_buffer& create = frame->ib_updates[i];
			if(create.user_fn)
			{
				renderer_frame_data::unique_ibs_t::iterator it = frame->unique_ibs.find(create.ib->get_storage()->get_driver_object());
				renderer_frame_data::locked_ib& lv = it->second;
				void* out_buffer = (char*)lv.out_buffer;// + create.ib->get_storage()->get_buffer_offset();
				create.user_fn(create.ib->get_storage(), out_buffer, create.num_bytes);
			}
		}
	}
	
	void renderer::rt_create_vertex_buffer_storage(renderer_frame_data* frame, renderer_frame_data::create_vertex_buffer_storage& create)
	{
		graphics::vertex_buffer_storage::driver_object_ptr vertex_store(
			m_interface->create_vertex_buffer(create.num_bytes, 32, create.create_flags));
		create.store->set_driver_object(vertex_store);
		create.store->set_state(resource::s_created);
		renderer_frame_data::locked_vb lv;
		lv.store = create.store;
		lv.out_buffer = vertex_store->lock();
		frame->unique_vbs.insert(std::make_pair(vertex_store, lv));	
	}

	void renderer::rt_update_vertex_buffer_storage(renderer_frame_data* frame, renderer_frame_data::update_vertex_buffer& update)
	{
		// check this isn't already locked
		graphics::buffer_storage::driver_object_ptr obj = update.vb->get_storage()->get_driver_object();
		if(frame->unique_vbs.find(obj) == frame->unique_vbs.end())
		{
			renderer_frame_data::locked_vb lv;
			lv.store = update.vb->get_storage();
			lv.out_buffer = obj->lock();
			frame->unique_vbs.insert(std::make_pair(obj, lv));	
		}
	}

	void renderer::rt_update_index_buffer_storage(renderer_frame_data* frame, renderer_frame_data::update_index_buffer& update)
	{
		TYCHO_NOT_IMPLEMENTED;
	}
		
	void renderer::rt_create_index_buffer_storage(renderer_frame_data* frame, renderer_frame_data::create_index_buffer_storage& create)
	{
		graphics::index_buffer_storage::driver_object_ptr index_store(
			m_interface->create_index_buffer(create.format, create.num_indices, create.create_flags));
		create.store->set_driver_object(index_store);
		create.store->set_state(resource::s_created);
		renderer_frame_data::locked_ib li;
		li.store = create.store;
		li.out_buffer = index_store->lock();
		frame->unique_ibs.insert(std::make_pair(index_store, li));	
	}
		
	void renderer::rt_update_vertex_buffers(renderer_frame_data* frame)
	{
		TYCHO_ASSERT(frame);
		rt_check_thread();
		
		// issue all updates
		for(size_t i = 0; i < frame->vb_creates.size(); ++i)
		{
			renderer_frame_data::create_vertex_buffer_storage& create = frame->vb_creates[i];
			if(create.user_fn)
			{
				void* out_buffer = rt_get_vertex_buffer_out_buffer(frame, create);
				create.user_fn(create.store, out_buffer, create.num_bytes);
			}
		}

		for(size_t i = 0; i < frame->vb_updates.size(); ++i)
		{
			renderer_frame_data::update_vertex_buffer& create = frame->vb_updates[i];
			if(create.user_fn)
			{
				renderer_frame_data::unique_vbs_t::iterator it = frame->unique_vbs.find(create.vb->get_storage()->get_driver_object());
				renderer_frame_data::locked_vb& lv = it->second;
				void* out_buffer = (char*)lv.out_buffer;// + create.vb->get_storage()->get_buffer_offset();
				create.user_fn(create.vb, out_buffer, create.num_bytes);
			}
		}
	}
			
	void renderer::rt_process_render_target(renderer_frame_data::render_target_data& rt)
	{
		rt_check_thread();
	
		// perform dependency analysis to find other render targets that are 
		// render operations on this one need for input.
		
		// process render ops
		//  1) remove any operations that use resources that are not loaded yet. delayed until here as
		//     they may have got created in this frame.
		//	2) sort the render operations into optimal order
		for(size_t v = 0; v < rt.viewports.size(); ++v)
		{
			renderer_frame_data::render_target_data::viewport_data& vp = rt.viewports[v];
			for(size_t o = 0; o < vp.ops.size(); ++o)
			{
				const render_op_ptr& op = vp.ops[o];
				
				// check all resources are loaded
				bool all_loaded = true;
				for(size_t i = 0; i < render_op::MaxVertexBuffers; ++i)
				{
					if(!op->vertex_buffers[i])
						continue;
						
					const vertex_buffer_storage_ptr& vb = op->vertex_buffers[i]->get_storage();
					if(vb != 0)
					{
						if(!vb->is_loaded())
						{
							all_loaded = false;
							break;
						}						
					}
				}
				if(all_loaded)
				{
					for(size_t i = 0; i < render_op::MaxIndexBuffers; ++i)
					{
						if(!op->index_buffers[i])
							continue;
							
						const index_buffer_storage_ptr& ib = op->index_buffers[i]->get_storage();
						if(ib != 0)
						{
							if(!ib->is_loaded())
							{
								all_loaded = false;
								break;
							}						
						}
					}
				}
				if(all_loaded)
				{
					// check all texture references
					if(op->shader && op->shader_params)
					{
						const shader::parameter_buffer_layout_ptr& layout = op->shader->get_parameter_layout();
						shader::parameter_buffer_view view(op->shader_params, layout);
						for(int p = 0; p < layout->get_num_params(); ++p)
						{
							const shader::parameter_buffer_layout::parameter& param = layout->get_param(p);
							if(param.type == shader::spt_texture)
							{
								graphics::texture** tex = view.get_ptr<graphics::texture*>(p);
								if(tex && *tex)
								{
									if(!(*tex)->is_loaded())
									{
										all_loaded = false;
										break;
									}
								}
							}
						}
					}
				}
				op->disabled = !all_loaded;
			}
		}
	}
	
	
	void renderer::rt_render_render_target(renderer_frame_data::render_target_data& rt)
	{
		//TYCHO_SCOPED_GPU_EVENT(this, "render target");
		rt_check_thread();
		m_interface->begin_render(rt.rt);
		rt.rt->get_render_target_storage()->rt_clear();
		for(size_t i = 0; i < rt.viewports.size(); ++i)
		{
			renderer_frame_data::render_target_data::viewport_data& vp = rt.viewports[i];
			m_interface->set_viewport(vp.vp);
			for(size_t o = 0; o < vp.ops.size(); ++o)
			{
				if(!vp.ops[0]->disabled)
					m_interface->render(vp.ops[o]);
			}
		}
		m_interface->end_render();
	}

	void renderer::rt_check_thread()
	{
		//TODO
	}
	

	//////////////////////////////////////////////////////////////////////////
	// Debugging
	//////////////////////////////////////////////////////////////////////////
	
	/// marks the beginning of a GPU event.
	void renderer::begin_gpu_event(const char* name)
	{
		m_interface->begin_gpu_event(name);
	}
	
	/// marks the end of a GPU event
	void renderer::end_gpu_event()
	{
		m_interface->end_gpu_event();
	}

} // end namespace
} // end namespace

