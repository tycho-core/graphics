//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 6 April 2008 4:29:15 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __GAMECUBE_SHADER_H_9B727B2E_2242_4C99_916F_B10FB68734B6_
#define __GAMECUBE_SHADER_H_9B727B2E_2242_4C99_916F_B10FB68734B6_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/graphics_abi.h"
#include "graphics/shader/shader.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace shader
{

	/// this implements a shader based on the gamecube TEV units. it's not
	/// all in platform specific code itself as we want to emulate it on PC
	/// using vertex/pixel shader's or using the x86 dolphin emulator.
    class TYCHO_GRAPHICS_ABI gamecube_shader : public shader_base
    {
    public:
		enum gx_tev_stage
		{
			gx_tev_stage1
		};
		
		enum gx_tev_clr_arg
		{
			gx_cc_c0,
			gx_cc_c1,
			gx_cc_c2,
			gx_cc_cprev,
			gx_cc_a0,
			gx_cc_a1,
			gx_cc_a2,
			gx_cc_aprev,
			gx_cc_texa,
			gx_cc_rasc,
			gx_cc_one,
			gx_cc_half,
			gx_cc_konst,
			gx_cc_zero
		};
		
		enum gx_tev_alpha_arg
		{
			gx_ca_a0,
			gx_ca_a1,
			gx_ca_a2,
			gx_ca_aprev,
			gx_ca_texa,
			gx_ca_rasa,
			gx_ca_konst,
			gx_ca_zero		
		};
		
		enum gx_tev_op
		{
			gx_tev_add,
			gx_tev_sub,
			gx_tev_comp_r8_gt,
			gx_tev_comp_r8_eq,
			gx_tev_comp_gr16_gt,
			gx_tev_comp_gr16_eq,
			gx_tev_comp_bgr24_gt,
			gx_tev_comp_bgr24_eq,
			gx_tev_comp_rgb8_gt,// = gx_tev_comp_a8_gt,
			gx_tev_comp_rgb8_eq //= gx_tev_comp_a8_eq		
		};
		
		enum gx_tev_bias
		{
			gx_tb_zero,
			gx_tb_addhalf,
			gx_tb_subhalf,
			gx_max_tevbias,		
		};
		
		enum gx_tev_scale
		{
			gx_cs_scale_1,
			gx_cs_scale_2,
			gx_cs_scale_4,
			gx_cs_divide_2,
			gx_max_tevscale		
		};
		
		enum gx_bool
		{
			gx_false = 0,
			gx_true = 1
		};
		
		enum gx_tev_reg_id
		{
			gx_tevreg0,
			gx_tevreg1,
			gx_tevreg2,
			gx_tevprev,
			gx_max_tevreg		
		};
		
		
		struct gx_tev_op_setup
		{
			gx_tev_stage	stage;
			gx_tev_op		op;
			gx_tev_bias		bias;
			gx_tev_scale	scale;
			gx_bool			clamp;
			gx_tev_reg_id	out_reg;
		};
		
		struct tev_stage_setup
		{
			gx_tev_stage		stage;
			gx_tev_clr_arg		clr[4];
			gx_tev_op_setup		clr_op;
			gx_tev_alpha_arg	alpha[4];
			gx_tev_op_setup		alpha_op;
		};
		
    public:
		/// create from a definition file
		void load(const char*) {}
		
		/// \name \ref graphics::shader interface
		//@{ 
		virtual int get_num_input_semantics() const { return 0; }
		virtual const vertex_format::semantic_decl* get_input_semantic(int) const { return 0; }
		virtual const parameter_buffer_ptr get_shader_parameters() const { return 0; }
		virtual parameter_buffer_ptr create_default_table() const { return 0; }
		virtual const parameter_buffer_layout_ptr get_parameter_layout() const { return 0; }
		virtual void render_setup(graphics::shader::parameter_buffer_ptr) {}
		virtual void bind(parameter_object_lookup_table_ptr) {}
		//@}
		
    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_GRAPHICS_ABI, gamecube_shader)
    };
    
    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_GRAPHICS_ABI, gamecube_shader)    
    
} // end namespace
} // end namespace
} // end namespace

#endif // __GAMECUBE_SHADER_H_9B727B2E_2242_4C99_916F_B10FB68734B6_
