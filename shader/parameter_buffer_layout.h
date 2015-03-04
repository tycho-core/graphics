//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 2:24:56 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PARAMETER_BUFFER_LAYOUT_H_A8269227_B183_4301_99B4_6C350E3A5924_
#define __PARAMETER_BUFFER_LAYOUT_H_A8269227_B183_4301_99B4_6C350E3A5924_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/shader/parameters.h"
#include "graphics/resource.h"
#include "core/perfect_hash.h"
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{
	
	/// A parameter buffer layout fully describes the arrangement of all parameters at the byte level. It
	/// also offers a fast lookup from parameter name to its description. i.e. for the parameters listed as
	/// <pre>
	/// name		type		size		offset
	/// ------------------------------------------
	/// foo         rgba        4           0	
	/// bar         vector4     16          4
	/// ike         int         4           20
	/// </pre>
	/// it is laid out in the buffer as
	/// <pre>
	/// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
	/// | foo | |          bar                | | ike |
	/// </pre>
	/// To create the layout you push parameters into the buffer in the order that you want them in there
	/// using the \ref add_parameter function, then when you have added them all call finalise to create the
	/// final layout. Until finalise is called any functions taking a key as text ( \ref get_param_offset etc)
	/// will fail. After calling finalise you cannot add any more parameters.
	class TYCHO_GRAPHICS_ABI parameter_buffer_layout : public graphics::resource
	{
	public:
		/// information about a single parameter in the buffer.
		struct parameter
		{
			std::string			name;		///< name of the parameter
			parameter_type	    type;		///< type of the parameter
			parameter_modifier	modifiers;	///< modifiers that apply to the parameter
			int					offset;		///< offset into the parameter buffer
			int					size;		///< number of bytes used in the parameter buffer
		};
	
	public:
		/// constructor
		parameter_buffer_layout();
		
		/// destructor
		~parameter_buffer_layout();
		
		/// add a new parameter to the map.
		void add_parameter(const char *name, parameter_type type, parameter_modifier modifiers);
		
		/// finalise the layout, no more parameters can be added after this. note after this the
		/// parameters will not necessarily be in the same order they were added.
		void finalise();
		
		/// \returns the size of the buffer need to hold all the parameters.
		int get_required_size() const;

		/// \returns the offset to a particular parameter
		int get_param_offset(int param_index) const;
		
		/// \returns the offset to a particular parameter
		int get_param_offset(const char *key, int len) const;
		
		/// \returns the number of parameters in the buffer
		int get_num_params() const;
		
		/// \returns the i'th parameter
		const parameter& get_param(int i) const;

		/// \returns look up parameter by name
		const parameter* get_param(const char*, int len) const;
		
	private:
		/// \returns index to pass name of -1 if not found.
		int lookup_param(const char*);
		
		/// non copyable
		void operator=(const parameter_buffer_layout &);
		
	private:		
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, parameter_buffer_layout);
		std::vector<parameter>				m_params;			///< map of all parameters		
		core::perfect_hash::hash_function*	m_lookup;			///< name to index lookup
		int									m_buffer_size;		///< size of buffer to hold all parameters
	};

	TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, parameter_buffer_layout);

} // end namespace
} // end namespace
} // end namespace

#endif // __PARAMETER_BUFFER_LAYOUT_H_A8269227_B183_4301_99B4_6C350E3A5924_
