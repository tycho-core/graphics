//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 23 April 2008 1:56:00 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PARAMETER_BINDINGS_H_3234D93B_DDC9_433E_8830_C5791264F00C_
#define __PARAMETER_BINDINGS_H_3234D93B_DDC9_433E_8830_C5791264F00C_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/resource.h"
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{
	struct TYCHO_GRAPHICS_ABI parameter_binding
	{
		std::string shader_param;
		std::string src_object;
		std::string src_param;
	};

	/// This binds names in the input table (set by user code) to parameters in 
	/// the shaders.
    class TYCHO_GRAPHICS_ABI parameter_bindings : public graphics::resource
    {    
    public:
		/// Bind a shader parameter to a published parameter object.
		/// \param shader_param Name of the shader parameter to bind.
		/// \param src_object Name of the source object to bind to (i.e. scene.camera).
		/// \param src_param Name of the parameter in the object to bind to.
		void bind(const char* shader_param, const char* src_object, const char* src_param);
		
		/// \returns The number of bindings in the set, this should match the number of parameters the shader exposes
		int get_num_bindings() const;
		
		/// \returns The i'th binding. const version.
		const parameter_binding& get_binding(int i) const;

		/// \returns The i'th binding. non-const version.
		parameter_binding& get_binding(int i);
		
    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, parameter_bindings)
		std::vector<parameter_binding> m_bindings;    
    };

    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, parameter_bindings)

} // end namespace
} // end namespace
} // end namespace

#endif // __PARAMETER_BINDINGS_H_3234D93B_DDC9_433E_8830_C5791264F00C_
