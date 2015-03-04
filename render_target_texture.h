//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 5 July 2008 3:42:59 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDER_TARGET_TEXTURE_H_D3E8B48D_77DC_4B6D_A633_DA430C5AA04A_
#define __RENDER_TARGET_TEXTURE_H_D3E8B48D_77DC_4B6D_A633_DA430C5AA04A_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/texture.h"
#include "graphics/render_target.h"
#include "graphics/render_target_storage.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// 
	class render_target_texture_storage :
		public texture_storage,
		public render_target_storage
	{	
	public:
		virtual ~render_target_texture_storage() {}
	private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, render_target_texture_storage)    
	};
	
	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, render_target_texture_storage)
	
	/// texture that can be rendered to.
    class TYCHO_GRAPHICS_ABI render_target_texture :
		public render_target,
		public texture_base
    {
    public:
		/// constructor
		render_target_texture(renderer*, int width, int height);
		
		/// destructor
		~render_target_texture();		
				
		/// \name texture_base interface
		//@{
		virtual graphics::texture_storage_ptr get_texture_storage()
			{ return m_storage; }

		virtual graphics::render_target_storage_ptr get_render_target_storage()
			{ return m_storage; }
		
		virtual int get_width() const
			{ return m_width; }

		virtual int get_height() const 
			{ return m_height; }
			
		virtual int get_num_mips() const
			{ return 1; }
		//@}
		
	private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, render_target_texture)    
	    friend class renderer;		
		void set_storage(render_target_texture_storage_ptr storage)
			{ m_storage = storage; }			
			
		render_target_texture_storage_ptr m_storage;
		int m_width;
		int m_height;
    };

    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, render_target_texture)

} // end namespace
} // end namespace

#endif // __RENDER_TARGET_TEXTURE_H_D3E8B48D_77DC_4B6D_A633_DA430C5AA04A_
