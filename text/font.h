//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 30 May 2008 12:58:20 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __FONT_H_7B76C846_23B8_4E07_BF02_1B634D0EC34D_
#define __FONT_H_7B76C846_23B8_4E07_BF02_1B634D0EC34D_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/text/forward_decls.h"
#include "core/noncopyable.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace text
{
		
	/// font handle, used to specify which font to use during string rendering
	/// operations.
    class TYCHO_GRAPHICS_ABI font : core::noncopyable
    {
    public:
		/// constructor
		font();
		
		/// destructor
		~font();
		
		/// \returns unique 32 bit id for this font
		int get_unique_id() const
			{ return m_id; }
		
    private:
		friend class font_manager;
		FT_FaceRec_*	  m_face;
		std::vector<char> m_data;
		int				  m_id;
		static int		  ms_cur_id;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __FONT_H_7B76C846_23B8_4E07_BF02_1B634D0EC34D_
