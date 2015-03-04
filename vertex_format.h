//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:04:45 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VERTEX_FORMAT_H_1E8FF55B_A114_4F84_919D_735F8A119692_
#define __VERTEX_FORMAT_H_1E8FF55B_A114_4F84_919D_735F8A119692_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/resource.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{

	/// description of the layout of a vertex. this is split into two sections,
	/// the storage types and a separate semantic map which may be larger than
	/// the number of storage types as multiple semantics may by packed into a 
	/// single storage slot. this is relatively complex as it supports storing
	/// more than one semantic per vertex component (i.e. vertex component type
	/// is a vector4 and you put position and ambient occlusion info into it)
	/// <pre>
	/// 	component_decl c[] = {
	/// 		{ ct_float, 4 },		
	/// 		{ ct_rgb,   1 }
	/// 	};
	/// 		
	/// 	semantic_decl s[] = {
	/// 		{ s_position, 0, c_xyz },
	/// 		{ s_ambient_occlusion, 0, c_w },
	/// 		{ s_diffuse, 1, c_rgba }
	///		};
	/// </pre>
	/// this complicates the definition but means we have enough information 
	/// to generically convert between different formats during the platform
	/// specific vertex optimise stage as well as simplifying tools writing 
	/// as the actual location and type is abstracted away.
    class TYCHO_GRAPHICS_ABI vertex_format : public graphics::resource
    {
    public:
		/// element components, multiple flags may be or'd together to indicate a 
		/// semantic uses all of them.
		enum element
		{
			c_x = 0,
			c_y = 1,
			c_z = 2,
			c_w = 4,

			c_r = c_x,
			c_g = c_y,
			c_b = c_z,
			c_a = c_w,
			
			c_xy = c_x | c_y,
			c_zw = c_z | c_w,
			c_xyz = c_x | c_y | c_z,
			c_xyzw = c_x | c_y | c_z | c_w,			
			
			c_rgb = c_xyz,
			c_rgba = c_xyzw
		};
		
		
		/// basic type for the component
		enum component_type
		{
			ct_invalid,
			ct_float,
			ct_double,
			ct_int8,					
			ct_int16,					
			ct_int32,					
			ct_uint8,					
			ct_uint16,					
			ct_uint32,					
			ct_fixed32_16_16,		///< unsigned 32 bit fixed point 16.16 format
			ct_fixed16_8_8,			///< unsigned 32 bit fixed point 16.16 format
			ct_normalized_int8,		///< [-1, 1] in 8 bit int
			ct_normalized_int16,	///< [-1, 1] in 16 bit int
			ct_normalized_int32,	///< [-1, 1] in 32 bit int
			ct_clr,
			ct_hdr_clr,						
			// this is a tuple, whats its basic type? ct_ubyte4 // (etc..)
		};
		
// 		enum component_type
// 		{
// 			ct_float1,
// 			ct_float2,
// 			ct_float3,
// 			ct_float4,
// 			ct_double1,
// 			ct_double2,
// 			ct_double3,
// 			ct_double4,
// 			ct_bool1,
// 			ct_bool2,
// 			ct_bool3,
// 			ct_bool4,
// 			ct_int8_1,
// 			ct_int8_2,
// 			ct_int8_3,
// 			ct_int8_4,
// 			ct_uint8_1,
// 			ct_uint8_2,
// 			ct_uint8_3,
// 			ct_uint8_4,
// 			ct_int16_1,
// 			ct_int16_2,
// 			ct_int16_3,
// 			ct_int16_4,
// 			ct_uint16_1,
// 			ct_uint16_2,
// 			ct_uint16_3,
// 			ct_uint16_4,
// 			ct_int32_1,
// 			ct_int32_2,
// 			ct_int32_3,
// 			ct_int32_4,
// 			ct_uint32_1,
// 			ct_uint32_2,
// 			ct_uint32_3,
// 			ct_uint32_4,
// 			ct_rgb,
// 			ct_rgba,
// 			ct_ubyte4			
// 			
// 			// and variants for, fixed point, normalized etc.
// 		};
		
		/// semantic uses
		enum semantic_type
		{
			s_invalid,
			s_position,
			s_texcoord,
			s_tangent,
			s_normal,
			s_binormal,
			s_diffuse,
			s_specular,
			s_bone_indices,
			s_bone_weights,
			s_ambient_occlusion
		};
		
		static int get_component_size(component_type);
		static const char* get_component_string(component_type);
		static const char* get_semantic_string(semantic_type);
		
		
		/// 
		struct component_decl
		{
			component_type type;
			int			   size;
		};

		static component_decl make_component_decl(component_type t, int s)
		{
			 component_decl cd;
			 cd.type = t;
			 cd.size = s;
			 return cd;
		}
		
		struct semantic_decl
		{
			semantic_type type;		///< semantic type
			int		 index;			///< index to differentiate multiple streams of the same type 
			int		 component;		///< index of component this is stored in
			int		 elements;		///< which elements in the component it uses
		};		

		static semantic_decl make_semantic_decl(semantic_type type, int index, int component, int elements)
		{
			semantic_decl s;			
			s.type = type;
			s.index = index;
			s.component = component;
			s.elements = elements;
			return s;
		}
		
    public:
		/// constructor
		vertex_format(const component_decl *, const semantic_decl *);		
		
		/// default constructor
		vertex_format();
		
		/// query if the format contains a particular semantic. O(N), N = number of semantics
		bool has_semantic(semantic_type, int index) const;
	
		/// query if the format contains a particular semantic
		const semantic_decl* get_semantic(semantic_type type, int index) const;
	
		/// add another component
		int add_component(const component_decl &);
		
		/// add another semantic
		int add_semantic(const semantic_decl &);
		
		/// \returns the number of components in the vertex
		int get_num_components() const;

		/// \returns the number of components in the vertex
		int get_num_semantics() const;
		
		/// \returns the offset in bytes from the start of the vertex to this component
		int get_component_offset(int i) const;

		/// \returns the i'th component in the vertex
		const component_decl& get_component(int i) const;

		/// \returns the i'th semantic in the vertex
		const semantic_decl& get_semantic(int i) const;
					
		/// \returns the size of the vertex in bytes
		int size() const;
		
		/// \returns a guid unique to the component layout, guaranteed to be the same for 
		///          2 different objects but with matching components.
		core::uint32 get_components_guid();
		 
	private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, vertex_format)
		static const int MaxComponents = 8;
		static const int MaxSemantics = 8;
		
		int			   m_num_components;
		int			   m_num_semantics;
		component_decl m_components[8];
		int			   m_offsets[8];
		int			   m_cur_offset;
		semantic_decl  m_semantics[8];			
    };
    
    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, vertex_format)

} // end namespace
} // end namespace

#endif // __VERTEX_FORMAT_H_1E8FF55B_A114_4F84_919D_735F8A119692_
