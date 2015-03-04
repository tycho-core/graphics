const char system_single_clr[] = "\
// simple single colour per vertex shader \n\
float4x4 world_view_proj; \n\
float4x4 world_matrix; \n\
 \n\
// light direction and colour \n\
float4 light_dir; \n\
float4 light_clr; \n\
float4 obj_clr; \n\
 \n\
struct VS_OUTPUT \n\
{ \n\
	float4 position : POSITION; \n\
	float4 diffuse  : COLOR0; \n\
}; \n\
	 \n\
struct PS_OUTPUT \n\
{ \n\
	float4 clr : COLOR0; \n\
}; \n\
 \n\
PS_OUTPUT ps_main(VS_OUTPUT input) \n\
{ \n\
	PS_OUTPUT output; \n\
	output.clr = input.diffuse; \n\
	return output; \n\
} \n\
 \n\
VS_OUTPUT vs_main(float4 vPos : POSITION, float4 vNormal : NORMAL) \n\
{ \n\
	float3 ambient = float3(0.5, 0.5, 0.5); \n\
	VS_OUTPUT output; \n\
	float4 world_pos = mul(vPos, world_matrix); \n\
	output.position = mul(world_pos, world_view_proj); \n\
	float3 diffuse = ambient + light_clr * dot((float3)vNormal, -(float3)light_dir); \n\
	output.diffuse = float4(diffuse.x, diffuse.y, diffuse.z, 1) * obj_clr; \n\
	return output; \n\
} \n\
 \n\
 \n\
";
