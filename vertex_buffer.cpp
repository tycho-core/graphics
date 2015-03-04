//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:59:25 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "vertex_buffer.h"
#include "vertex_format.h"
#include "vertex_buffer_storage.h"
#include "core/memory.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, vertex_buffer);

	/// constructor
	vertex_buffer::vertex_buffer(vertex_format* fmt, int num_verts) : 
		m_format(fmt),
		m_store(0),
		m_num_verts(num_verts)
	{	
	}

	bool vertex_buffer::create(vertex_format* f, vertex_buffer_storage_ptr s)
	{	
		TYCHO_ASSERT(s);
		m_format = f;
		m_store = s;
		m_num_verts = get_bytes_used() / f->size();
		return true;
	}

	/// destructor
	vertex_buffer::~vertex_buffer()
	{
	}

	/// format of the vertex buffer
	const vertex_format* vertex_buffer::get_format() const
	{ 
		return m_format; 
	}

	/// lock the buffer
	void* vertex_buffer::lock()
	{
		TYCHO_ASSERT(m_store);
		return m_store->lock();
	}
	
	/// unlock the buffer
	void vertex_buffer::unlock()
	{
		TYCHO_ASSERT(m_store);
		return m_store->unlock();	
	}

	/// get the size of the vertex buffer in bytes
	int vertex_buffer::get_bytes_used() const
	{
		TYCHO_ASSERT(m_store);
		return m_store->size();
	}


	/// \returns the number of vertices there is space for in the buffer
	int vertex_buffer::get_num_verts() const
	{
		return m_num_verts;
	}


	/////////////////////////////////////////////////////////////////////////////////////
	// Vertex buffer format conversion support. 
	// \todo should be broken out into standalone helper functions
	// \todo support for optimised conversions for common cases
	/////////////////////////////////////////////////////////////////////////////////////
	
	template<class SrcType, class DstType>		
	void convert(const void *src, void *dst)
	{
		*((DstType*)dst) = (DstType)*((const SrcType*)src);
	}

	typedef void (*conversion_func)(const void *, void *);		
	template<class Type>
	conversion_func standard_conversion(vertex_format::component_decl dst_decl)
	{
		switch(dst_decl.type)
		{
			case vertex_format::ct_float	: return &convert<Type, float>;  break;
			case vertex_format::ct_double	: return &convert<Type, double>; break;
			case vertex_format::ct_int8		: return &convert<Type, core::int8>; break;
			case vertex_format::ct_int16	: return &convert<Type, core::int16>; break;
			case vertex_format::ct_int32	: return &convert<Type, core::int32>; break;
			case vertex_format::ct_uint8	: return &convert<Type, core::uint8>; break;
			case vertex_format::ct_uint16	: return &convert<Type, core::uint16>; break;
			case vertex_format::ct_uint32	: return &convert<Type, core::uint32>; break;
			case vertex_format::ct_clr		: return &convert<Type, core::uint8>;  break;
			case vertex_format::ct_hdr_clr	: return &convert<Type, float>; break;
                
            default : TYCHO_NOT_IMPLEMENTED; break;
		}	
		
		return 0;
	}
	
	/// double dispatch to find an appropriate conversion function
	conversion_func convert(vertex_format::component_decl src_decl, vertex_format::component_decl dst_decl)
	{
		// per type conversion
		switch(src_decl.type)
		{
			case vertex_format::ct_hdr_clr	: 
			case vertex_format::ct_float	: return standard_conversion<float>(dst_decl);  break;
			case vertex_format::ct_double	: return standard_conversion<double>(dst_decl); break;
			case vertex_format::ct_int8		: return standard_conversion<core::int8>(dst_decl);  break;
			case vertex_format::ct_int16	: return standard_conversion<core::int16>(dst_decl);  break;
			case vertex_format::ct_int32	: return standard_conversion<core::int32>(dst_decl);  break;
			case vertex_format::ct_clr		: 
			case vertex_format::ct_uint8	: return standard_conversion<core::uint8>(dst_decl);  break;			
			case vertex_format::ct_uint16	: return standard_conversion<core::uint16>(dst_decl);  break;
			case vertex_format::ct_uint32	: return standard_conversion<core::uint32>(dst_decl);  break;
                
            default: TYCHO_NOT_IMPLEMENTED; break;
		}
		
		return 0;
	}

	
	int vertex_buffer::get_buffer_offset() const
	{	
		return m_store->get_buffer_offset();
	}	

	
	/// copy the input vertices to the output possibly converting between types
	/// \todo this needs some optimisation loving at some point, slow as at the mo.
	bool vertex_buffer::write(const vertex_format &src_format, 
							  const void* src_ptr, 
							  const vertex_format &dst_format, 
							  void* dst_ptr, 
							  int num_verts)
	{
		// loop over the source formats and find its counterpart in the destination format, then check to see
		// that they are have the same size and are convertible. this builds the local list of writer functions
		// that are then called for every vertex to map it into the new vertex format.

		// conversion functions to run in order over each vertex
		struct conversion
		{
			int src_offset;
			int dst_offset;
			int src_size;
			int dst_size;
			int size;
			conversion_func func;
		};
		const int MaxConverters = 32;
		conversion converters[MaxConverters] = { {0} };
		int num_converters = 0;
		bool needs_converting = false;
		// map src semantic to dst semantic		
		for(int i = 0; i < src_format.get_num_semantics(); ++i)
		{
			const vertex_format::semantic_decl &s = src_format.get_semantic(i);
			const vertex_format::semantic_decl *dst_s = dst_format.get_semantic(s.type, s.index);
			
			// its ok for the destination not to exist, we just ignore this semantic in that case.
			if(dst_s)
			{
				// check they are in the same index slot 
				if(s.index == dst_s->index)
				{				
					// check they have the same amount of elements
					const vertex_format::component_decl &src_decl = src_format.get_component(s.component);
					const vertex_format::component_decl &dst_decl = dst_format.get_component(dst_s->component);
					if(src_decl.size == dst_decl.size)
					{
						// finally hunt for a conversion function for these
						conversion_func f = convert(src_decl, dst_decl); 
						if(!f)
						{
							/// \todo  error no converter
							continue;
						}
						
						converters[num_converters].src_offset = src_format.get_component_offset(s.component);
						converters[num_converters].src_size   = vertex_format::get_component_size(src_decl.type);
						converters[num_converters].dst_offset = dst_format.get_component_offset(dst_s->component);
						converters[num_converters].dst_size   = vertex_format::get_component_size(dst_decl.type);
						converters[num_converters].size = src_decl.size;
						converters[num_converters].func = f;
						
						TYCHO_ASSERT(num_converters != MaxConverters);
						++num_converters;

						// we need to go the slow route as something needs to be converted						
						needs_converting |= (src_decl.type != dst_decl.type);
					}
				}
			}
		}
		
		if(needs_converting)
		{
			const char *src = (const char*)src_ptr;
			char *dst = (char*)dst_ptr;
			int src_size = src_format.size();
			int dst_size = dst_format.size();
			for(int i = 0; i < num_verts; ++i)
			{
				/// \todo reorder converters so they are in the same order as the components in the destination vertex 
				///       so we play nice with write combiner units that require writes in strictly ascending order
				///		  of memory.
				for(int c = 0; c < num_converters; ++c)
				{
					const conversion &conv = converters[c];
					const char* src_p = src + conv.src_offset;
					char* dst_p = dst + conv.dst_offset;
					for(int p = 0; p < conv.size; ++p)
					{
						conv.func(src_p + conv.src_offset, dst_p + conv.dst_offset);
						src_p += conv.src_size;
						dst_p += conv.dst_size;
					}
				}
				
				// next vertex
				src += src_size;
				dst += dst_size;
			}						
		}
		else
		{
			// identical just memcpy
			const int num_bytes = src_format.size() * num_verts;
			core::mem_cpy(dst_ptr, src_ptr, num_bytes);
		}
		
		return false;
	}

} // end namespace
} // end namespace
