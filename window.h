//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 8:04:22 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __WINDOW_H_3949F974_8F21_4F36_823B_9C14A2790321_
#define __WINDOW_H_3949F974_8F21_4F36_823B_9C14A2790321_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/forward_decls.h"
#include "graphics/render_target.h"
#include "graphics/render_target_storage.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// device specific window interface
	class window_storage : public render_target_storage
	{
	private:
		friend class renderer;			
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, window_storage)

		/// called before rendering starts
		virtual void rt_begin_scene() = 0;
		
		/// called after rendering has completed
		virtual void rt_end_scene() = 0;
		
		/// clear the render target
		virtual void rt_clear() = 0;
		
		/// present the target to a output device, only valid for window render targets.
		virtual void rt_present() = 0;		
	};
	
	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, window_storage)
	
	/// connected to a physical display device
	class TYCHO_GRAPHICS_ABI window : public render_target
	{
	public:
		window(int width, int height);
		
		/// \name
		//@{ render_target interface
		virtual int get_width() const;
		virtual int get_height() const;	        
		virtual graphics::render_target_storage_ptr get_render_target_storage();
		//@}

	
	private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, window)
		friend class renderer;
		void set_storage(render_target_storage_ptr storage)
			{ m_storage = storage; }			
			
		graphics::render_target_storage_ptr m_storage;
		int m_width;
		int m_height;
	};
	
	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, window)

} // end namespace
} // end namespace

#endif // __WINDOW_H_3949F974_8F21_4F36_823B_9C14A2790321_
