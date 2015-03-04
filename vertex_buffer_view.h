//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 4 April 2008 2:00:52 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VERTEX_BUFFER_VIEW_H_00D51E35_5BB5_4900_A20C_134B14A38E8A_
#define __VERTEX_BUFFER_VIEW_H_00D51E35_5BB5_4900_A20C_134B14A38E8A_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/vertex_format.h"
#include "math/vector3.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	class vertex_buffer;

	/// this provides a safe interface to read and write from vertex buffers. all accesses are
	/// validated against the vertex format if asserts are enabled. This hides from the user the
	/// complexity of what type the semantics are and where they live in the vertex. standard conversions
	/// are allowed to convert between the type passed in and its actual type in the vb. 
	/// 
	/// there are accessors defined to retrieve each in normalised form, this is the largest possible
	/// type for that semantic and as such can be modified and restored with possibly some small 
	/// inaccuracy. when speed and/or complete accuracy are needed then the type specific accessors 
	/// should be used although this will complicate the code due to needing to support all combinations.
    class TYCHO_GRAPHICS_ABI vertex_buffer_view
    {
    public:
		/// constructor
		vertex_buffer_view(vertex_buffer *);				
		
		/// destructor
		~vertex_buffer_view();
		
		void write(vertex_format::semantic_type, char);
		void write(vertex_format::semantic_type, float);
		void write(vertex_format::semantic_type, int);
		void write(vertex_format::semantic_type, double);
		void write(vertex_format::semantic_type, math::vector3f);
		
    private:
		//vertex_buffer* m_vertex_buffer;
		
		// 2d array of semantic and index to map to a target 
    };

} // end namespace
} // end namespace

#endif // __VERTEX_BUFFER_VIEW_H_00D51E35_5BB5_4900_A20C_134B14A38E8A_
