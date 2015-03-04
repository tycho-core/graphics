//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:04:45 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "vertex_format.h"
#include "core/debug/assert.h"
#include "core/memory.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, vertex_format);

	/// constructor
	vertex_format::vertex_format(const component_decl *components, const semantic_decl *semantics) :
		m_num_components(0),
		m_num_semantics(0),
		m_cur_offset(0)
	{
		core::mem_zero(m_offsets, sizeof(m_offsets));
		while(components->type != ct_invalid)
		{
			// check they have not overrun the number of components allowed
			TYCHO_ASSERT(m_num_components < MaxComponents);			
			m_components[m_num_components] = *components;
			m_offsets[m_num_components] = m_cur_offset;
			m_cur_offset += get_component_size(components->type) * components->size;
			++m_num_components;
			++components;
		}
		
		while(semantics->type != s_invalid)
		{
			// check they have not overrun the number of semantics allowed
			TYCHO_ASSERT(m_num_semantics < MaxSemantics);			
			m_semantics[m_num_semantics++] = *semantics;
			++semantics;			
		}		
	}
	
	vertex_format::vertex_format() :
		m_num_components(0),
		m_num_semantics(0),
		m_cur_offset(0)
	{
	}
	
	/// query if the format contains a particular semantic
	bool vertex_format::has_semantic(semantic_type type, int index) const
	{
		for(int i = 0; i < m_num_semantics; ++i)
		{
			if(m_semantics[i].type == type && m_semantics[i].index == index)
				return true;
		}
		return false;
	}

	/// query if the format contains a particular semantic
	const vertex_format::semantic_decl* vertex_format::get_semantic(semantic_type type, int index) const
	{
		for(int i = 0; i < m_num_semantics; ++i)
		{
			const semantic_decl *decl = &m_semantics[i];
			if(decl->type == type && decl->index == index)
				return decl;
		}
		return 0;
	}
		
	/// add another component
	int vertex_format::add_component(const component_decl &c)
	{
		TYCHO_ASSERT(m_num_components < MaxComponents);
		m_components[m_num_components] = c;
		m_offsets[m_num_components] = m_cur_offset;
		m_cur_offset += get_component_size(c.type) * c.size;
		
		++m_num_components;
		return m_num_components-1;
	}
	
	/// add another semantic
	int vertex_format::add_semantic(const semantic_decl &s)
	{
		TYCHO_ASSERT(m_num_semantics < MaxSemantics);
		m_semantics[m_num_semantics++] = s;
		return m_num_semantics-1;
	}
	
	/// \returns the number of components in the vertex
	int vertex_format::get_num_components() const
	{ 
		return m_num_components; 
	}

	/// \returns the number of components in the vertex
	int vertex_format::get_num_semantics() const
	{ 
		return m_num_semantics; 
	}

	/// \returns the i'th component in the vertex
	const vertex_format::component_decl& vertex_format::get_component(int i) const
	{
		TYCHO_ASSERT(i < m_num_components);
		return m_components[i];
	}

	/// \returns the i'th semantic in the vertex
	const vertex_format::semantic_decl& vertex_format::get_semantic(int i) const
	{
		TYCHO_ASSERT(i < m_num_semantics);
		return m_semantics[i];
	}
				
	/// \returns the offset in bytes from the start of the vertex to this component
	int vertex_format::get_component_offset(int i) const
	{
		TYCHO_ASSERT(i < m_num_components);
		return m_offsets[i];	
	}
				
	/// return the size of the vertex in bytes
	int vertex_format::size() const
	{
		int size = 0;
		for(int i = 0; i < m_num_components; ++i)
		{
			const component_decl &c = get_component(i);
			size += get_component_size(c.type) * c.size;
		}
		
		return size;
	}
	
	/// \returns the hash of all the components
	core::uint32 vertex_format::get_components_guid()
	{
		// can't do a hash like we want 
		return 0;//core::hash(&m_components[0], sizeof(component_decl) * m_num_components); 
	}
	
	//////////////////////////////////////////////////////////////////////////
	// static functions
	//////////////////////////////////////////////////////////////////////////
	
	int vertex_format::get_component_size(component_type c)
	{
		switch(c)
		{
			case ct_float			: return 4;
			case ct_double			: return 8;
			case ct_int8 			: return 1;
			case ct_int16			: return 2;
			case ct_int32			: return 4;
			case ct_fixed32_16_16	: return 4;
			case ct_fixed16_8_8		: return 2;
			case ct_normalized_int8	: return 1;
			case ct_normalized_int16: return 2;
			case ct_normalized_int32: return 4;
			case ct_clr				: return 1;
			case ct_hdr_clr			: return 4;
//			case ct_ubyte4			: return 4;
                
            default: TYCHO_ASSERT(!"invalid component type!"); break;

		}
		
		return -1;
	}
	
	const char* vertex_format::get_component_string(component_type c)
	{
		switch(c)
		{
			case ct_invalid			: return "invalid";
			case ct_float			: return "float";
			case ct_double			: return "double";
			case ct_int8			: return "int8";
			case ct_int16			: return "int16";
			case ct_int32			: return "int32";
			case ct_fixed32_16_16	: return "fixed32_16_16";
			case ct_fixed16_8_8		: return "fixed_16_8_8";
			case ct_normalized_int8	: return "normalized_int8";
			case ct_normalized_int16: return "normalized_int16";
			case ct_normalized_int32: return "normalized_int32";
			case ct_clr				: return "clr";
			case ct_hdr_clr			: return "hdr_clr";
//			case ct_ubyte4			: return "ubyte4";
                
            default: TYCHO_ASSERT(!"invalid component type!"); break;
		}
		
		return "<!illegal component!>";
	}
	
	const char* vertex_format::get_semantic_string(semantic_type s)
	{
		switch(s)
		{
			case s_invalid			: return "invalid";
			case s_position			: return "position";
			case s_texcoord			: return "texcoord";
			case s_tangent			: return "tangent";
			case s_normal			: return "normal";
			case s_binormal			: return "binormal";
			case s_diffuse			: return "diffuse";
			case s_specular			: return "specular";
			case s_bone_indices		 : return "bone_indices";
			case s_bone_weights		 : return "bone_weights";
			case s_ambient_occlusion : return "ambient_occlusion";	
		}
		TYCHO_ASSERT("!invalid semantic type");
		return "<!illegal semantic!>";	
	}

} // end namespace
} // end namespace
