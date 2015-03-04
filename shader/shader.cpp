//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 7:00:06 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "shader.h"
#include "graphics/shader/parameter_buffer_view.h"
#include "graphics/shader/parameter_bindings.h"
#include "graphics/shader/parameter_object_lookup_table.h"
#include "core/memory.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace shader
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, tycho::graphics::shader::shader_base);

	void shader_base::bind(graphics::shader::parameter_buffer_ptr dst_buffer, parameter_object_lookup_table_ptr inputs, parameter_bindings_ptr bindings)
	{
		parameter_buffer_layout_ptr dst_layout = get_parameter_layout();
		
		// iterate over shader bindings resolving them to inputs
		for(int i = 0; i < bindings->get_num_bindings(); ++i)
		{
			const parameter_binding& binding = bindings->get_binding(i);			
			const parameter_buffer_layout::parameter *dst_param = dst_layout->get_param(binding.shader_param.c_str(), static_cast<int>(binding.shader_param.length()));
			if(dst_param)
			{
				parameter_buffer_view* iview = inputs->find(binding.src_object.c_str());
				if(iview)
				{	
					parameter_buffer_layout_ptr src_layout = iview->get_layout();
					parameter_buffer_ptr src_buffer = iview->get_buffer();
					const parameter_buffer_layout::parameter *src_param = src_layout->get_param(binding.src_param.c_str(), static_cast<int>(binding.src_param.length()));
					if(src_param)
					{
						/// \todo should check for possible conversions and use if available.
						if(src_param->type == dst_param->type)
						{
							// we got a match
							/// \todo this isn't going to be happy with non pod objects (i.e.textures)
							core::mem_cpy(dst_buffer->ptr() + dst_param->offset, src_buffer->ptr() + src_param->offset, dst_param->size);
						}
					}
				}
			}
		}
	}

} // end namespace
} // end namespace
} // end namespace
