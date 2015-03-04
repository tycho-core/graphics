//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:04:31 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VIEWPORT_H_B4280E8C_1D47_40C5_8981_ECED8BDD3872_
#define __VIEWPORT_H_B4280E8C_1D47_40C5_8981_ECED8BDD3872_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "math/vector2.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// defines the region of the render target to render to in normalised units.
    struct viewport
    {	
		viewport(const math::vector2f& origin, 
				 const math::vector2f& size, 
				 core::float32 min_depth, 
				 core::float32 max_depth) :
			m_origin(origin),
			m_size(size),
			m_min_depth(min_depth),
			m_max_depth(max_depth)
		{}
	
		viewport() : 
			m_origin(math::vector2f(0,0)), 
			m_size(math::vector2f(0,0)), 
			m_min_depth(0), 
			m_max_depth(0) {}

		void set_origin(const math::vector2f& o)
			{ m_origin = o; }
			
		void set_size(const math::vector2f&s)
			{ m_size = s; }
			
		void set_min_depth(core::float32 min_depth)
			{ m_min_depth = min_depth; }
			
		void set_max_depth(core::float32 max_depth)
			{ m_max_depth = max_depth; }
				
		const math::vector2f& get_origin() const	
			{ return m_origin; }
			
		const math::vector2f& get_size() const
			{ return m_size; }
			
		core::float32 get_min_depth() const
			{ return m_min_depth; }
			
		core::float32  get_max_depth() const
			{ return m_max_depth; }
				
	private:				
		math::vector2f m_origin;
		math::vector2f m_size;    
		core::float32  m_min_depth;
		core::float32  m_max_depth;
    };

} // end namespace
} // end namespace

#endif // __VIEWPORT_H_B4280E8C_1D47_40C5_8981_ECED8BDD3872_
