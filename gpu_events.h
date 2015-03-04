//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 10 August 2008 4:29:50 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __GPU_EVENTS_H_4FD0C785_4ABC_4121_B51D_8665087F64E7_
#define __GPU_EVENTS_H_4FD0C785_4ABC_4121_B51D_8665087F64E7_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"

//////////////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////////////

// GPU events crash DirectX June 2008 Pix, so disabled for now.
#define TYCHO_GPU_EVENTS_ENABLEDx

#ifdef TYCHO_GPU_EVENTS_ENABLED
#	define TYCHO_BEGIN_GPU_EVENT(_renderer, _name) _renderer->begin_gpu_event(_name)
#	define TYCHO_END_GPU_EVENT(_renderer) _renderer->end_gpu_event()
#   define TYCHO_SCOPED_GPU_EVENT(_renderer, _name) tycho::graphics::scoped_gpu_event gpuevent##__LINE__(_renderer, _name)
#else
#	define TYCHO_BEGIN_GPU_EVENT(_renderer, _name)
#	define TYCHO_END_GPU_EVENT(_renderer)
#   define TYCHO_SCOPED_GPU_EVENT(_renderer, _name)
#endif

//////////////////////////////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	class scoped_gpu_event
	{
	public:
		scoped_gpu_event(renderer* r, const char* name)
#ifdef TYCHO_GPU_EVENTS_ENABLED
            : m_renderer(r)
#endif
		{
			TYCHO_BEGIN_GPU_EVENT(r, name);
		}
		
		~scoped_gpu_event()
		{
			TYCHO_END_GPU_EVENT(m_renderer);
		}
	
	private:
#ifdef TYCHO_GPU_EVENTS_ENABLED
		renderer* m_renderer;
#endif
	};
	
} // end namespace
} // end namespace

#endif // __GPU_EVENTS_H_4FD0C785_4ABC_4121_B51D_8665087F64E7_
