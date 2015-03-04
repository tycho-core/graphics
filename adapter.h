//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 7:11:19 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __ADAPTER_H_50FD1E47_FA48_4A56_A533_125BAD5B0A24_
#define __ADAPTER_H_50FD1E47_FA48_4A56_A533_125BAD5B0A24_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/format.h"
#include "math/vector2.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	
	/// information about a single graphics adapter in the system
	class TYCHO_GRAPHICS_ABI adapter
	{
	public:
		/// 
		struct mode
		{
			int width;
			int height;
			int refresh_rate;
		};
		typedef std::vector<mode> modes;

		/// format supported by the adapter
		struct format
		{
			format() {}
			format(graphics::format fmt_, modes modes_)
			{
				fmt = fmt_;
				graphics_modes.swap(modes_);
			}
			graphics::format fmt;
			modes  graphics_modes;
		};
		typedef std::vector<format> formats;				
					
	public:
		/// constructor. 
		/// \param name		adapter name. stored by raw pointer so owner must make sure its around as long as this object is.
		/// \param fmts	formats supported by this adapter. the contents of the pass container are swapped into this object.
		adapter(const char *name, formats &fmts);
		
		/// \returns the name of the adapter
		const char* get_name() const
			{ return m_name; }
		
		/// \returns the number of formats supported by the adapter
		int get_num_formats() const;
		
		/// \returns the i'th supported format
		const format& get_format(int i) const;
						
	private:
		const char *m_name;
		formats		m_formats;
	};

} // end namespace
} // end namespace

#endif // __ADAPTER_H_50FD1E47_FA48_4A56_A533_125BAD5B0A24_
