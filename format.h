//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 8:00:47 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __FORMAT_H_6438832E_0731_4D2F_973C_643043ECF558_
#define __FORMAT_H_6438832E_0731_4D2F_973C_643043ECF558_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// 
	struct format
	{
		enum formats
		{
			unknown = 0,
			pixel_argb8888,
			pixel_xrgb8888,
			pixel_xrgb1555,					
			pixel_rgb565,	
			pixel_a8,
			depth_16bit,
			depth_24bit,
			depth_32bit,
			num_formats
		
		} value;
		
		format()  : value(unknown) {}
		format(formats f) : value(f) 
			{}

		bool operator==(format rhs) const
			{ return value == rhs.value; }

		bool operator!=(format rhs) const
			{ return value != rhs.value; }
			
		bool operator==(formats rhs) const
			{ return value == rhs; }

		bool operator!=(formats rhs) const
			{ return value != rhs; }
			
		format& operator=(format rhs)
		{
			value = rhs.value;
			return *this;
		}
		
		format& operator=(formats v)
		{
			value = v;
			return *this;
		}
		
	};
	


} // end namespace
} // end namespace

#endif // __FORMAT_H_6438832E_0731_4D2F_973C_643043ECF558_
