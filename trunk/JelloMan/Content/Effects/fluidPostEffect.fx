#include "blur.fx"

cbuffer cbPerObject
{
	matrix mtxWVP : WorldViewProjection;
    float3 vCamDir;
};

cbuffer cbRarely
{
    float2 t0;
};

Texture2D texBackbuffer;
Texture2D texDepthBackbuffer;
Texture2D texDepthFluids;

SamplerState mapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

RasterizerState rState
{
	FillMode = Solid;
	ScissorEnable = false;
};

BlendState blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
};

struct VertexShaderInput
{
    float3 position : POSITION0;
    float2 texCoord : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VertexShaderOutput VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;

    return output;	
};

float3 PS(VertexShaderOutput input) : SV_TARGET
{	
    //Depth test
    float depth_dest = texDepthBackbuffer.Sample(mapSampler, input.texCoord).r;
    //float depth_src = gaussblur(texDepthFluids, input.texCoord, t0);
    float depth_src = texDepthFluids.Sample(mapSampler, input.texCoord).r;

    clip(depth_dest - depth_src);

    //float3 normal = gaussblur(texBackbuffer, input.texCoord, t0);
    float3 normal = texBackbuffer.Sample(mapSampler, input.texCoord).xyz;
    normal = normalize(normal * 2.0f - 1.0f);
    
    float diff = dot(vCamDir, normal) * 0.5f + 0.5f;

    //return diff * float3(0.2f, 0.1f, 1.0f);
    return (normal + 1) / 2.0f;

    //return col;
};

technique10 tech1
{
	pass P0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_4_0, PS() ));
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
	}
}