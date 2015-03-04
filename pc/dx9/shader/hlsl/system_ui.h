const char system_ui[] = "\
// simple single colour per vertex shader \n\
float4x4	proj_matrix; \n\
float4x4	world_matrix; \n\
float4		obj_clr; \n\
texture	diffuse_texture; \n\
sampler diffuse_texture_sampler =  \n\
sampler_state \n\
{ \n\
	Texture = <diffuse_texture>; \n\
	MipFilter = LINEAR; \n\
	MinFilter = LINEAR; \n\
	MagFilter = LINEAR; \n\
}; \n\
 \n\
struct VS_OUTPUT \n\
{ \n\
	float4 position : POSITION; \n\
	float4 diffuse  : COLOR0; \n\
	float2 tex0		: TEXCOORD0; \n\
}; \n\
	 \n\
struct PS_OUTPUT \n\
{ \n\
	float4 clr : COLOR0; \n\
}; \n\
 \n\
VS_OUTPUT vs_main(float4 vPos : POSITION, float4 vNormal : NORMAL, float2 vTex0 : TEXCOORD0) \n\
{ \n\
	VS_OUTPUT output; \n\
	float4 world_pos = mul(vPos, world_matrix); \n\
	 \n\
	output.position.x = world_pos.x * 2 - 1; \n\
	output.position.y = world_pos.y * 2 - 1; \n\
	output.position.z = world_pos.z; \n\
	output.position.w = 1; \n\
	output.diffuse = obj_clr; \n\
	output.tex0 = vTex0; \n\
	return output; \n\
} \n\
 \n\
PS_OUTPUT ps_main(VS_OUTPUT input) \n\
{ \n\
	PS_OUTPUT output; \n\
	output.clr = tex2D(diffuse_texture_sampler, input.tex0);// * input.diffuse; \n\
	//output.clr.rgb = output.clr.aaa; \n\
	return output; \n\
} \n\
 \n\
 \n\
 \n\
";
