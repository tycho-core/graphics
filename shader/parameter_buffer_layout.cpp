//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 2:24:56 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "parameter_buffer_layout.h"
#include "core/debug/assert.h"
#include "core/memory.h"
#include "core/string.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
using namespace tycho::core;

#define DISABLE_PERFECT_HASH

namespace tycho
{
namespace graphics
{
namespace shader
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, parameter_buffer_layout);

	/// constructor
	parameter_buffer_layout::parameter_buffer_layout() :
		m_lookup(0),
		m_buffer_size(0)
	{}
	
	/// destructor
	parameter_buffer_layout::~parameter_buffer_layout()
	{
		core::safe_delete(m_lookup);
	}
	
	/// add a new parameter to the map.
	/// \todo align parameters correctly on a sensible boundary
	void parameter_buffer_layout::add_parameter(const char *name, parameter_type type, parameter_modifier modifiers)
	{
		TYCHO_ASSERT(!m_lookup);	// can't be used after finalisation
		parameter np;
		np.name		 = name;
		np.type		 = type;
		np.modifiers = modifiers;
		np.offset    = m_buffer_size;
		np.size		 = get_byte_size(type);		
		m_params.push_back(np);
		m_buffer_size += np.size;
	}
	
	/// finalise the layout, no more parameters can be added after this
	void parameter_buffer_layout::finalise()
	{
#ifndef DISABLE_PERFECT_HASH	
		std::vector<const char*> keys;
		for(size_t i = 0; i < m_params.size(); ++i)
			keys.push_back(m_params[i].name.c_str());
		perfect_hash::hash_function* func = perfect_hash::create_function(keys, perfect_hash::cf_minimal);
		TYCHO_ASSERT(func);
		if(func)
		{
			// find the index each key maps to
			std::vector<parameter> remap(keys.size());
			for(size_t i = 0; i < keys.size(); ++i)
			{
				int index = perfect_hash::lookup(*func, keys[i], strlen(keys[i]));
				TYCHO_ASSERT(index != -1);
				if(index != -1)
					remap[index] = m_params[i];
			}
			m_params.swap(remap);			
			m_lookup = func;
		}
#endif		
	}
	
	/// \returns the size of the buffer need to hold all the parameters.
	int parameter_buffer_layout::get_required_size() const
	{
		return m_buffer_size;
	}
	
	
	/// \returns the offset to a particular parameter
	int parameter_buffer_layout::get_param_offset(int param_index) const
	{
		const parameter* p = &m_params[param_index];
		if(!p)
			return -1;
		return p->offset;	
	}
	
	/// \returns the offset to a particular parameter
	int parameter_buffer_layout::get_param_offset(const char *key, int len) const
	{
		const parameter* p = get_param(key, len);
		if(!p)
			return -1;
		return p->offset;
	}

	/// \returns the number of parameters in the buffer
	int parameter_buffer_layout::get_num_params() const
	{
		return static_cast<int>(m_params.size());
	}
	
	/// \returns the i'th parameter
	const parameter_buffer_layout::parameter& parameter_buffer_layout::get_param(int i) const
	{
		return m_params[i];
	}

	/// \returns look up parameter by name
	const parameter_buffer_layout::parameter* parameter_buffer_layout::get_param(const char* key, int len) const
	{
#ifdef DISABLE_PERFECT_HASH	
		(void)len;
		for(size_t i = 0; i < m_params.size(); ++i)
		{
			if(core::strcmp(key, m_params[i].name.c_str()) == 0)
				return &m_params[i];
		}
		return 0;
#else
		TYCHO_ASSERT(m_lookup);
		int index = core::perfect_hash::lookup(*m_lookup, key, len);
		if(index == -1)
			return 0;
		if(core::strcmp(key, m_params[index].name.c_str()) != 0)
			return 0;			
		return &m_params[index];
#endif		
	}

} // end namespace
} // end namespace
} // end namespace

