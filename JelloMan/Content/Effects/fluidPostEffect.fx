#include "blur.fx"

cbuffer cbPerObject
{
	matrix mtxWVP : WorldViewProjection;
};

cbuffer cbRarely
{
    float2 t0;
};

Texture2D texBackbuffer;
Texture2D texDepth;

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
	BlendEnable[0] = FALSE;
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
    float depth = texBackbuffer.Sample(mapSampler, input.texCoord).a;
    float4 b = boxblur(texBackbuffer, input.texCoord, t0, depth);
    clip((b.r + b.g + b.b < 2.0f)? -1:1);
    clip(texDepth.Sample(mapSampler, input.texCoord).r < depth? -1 : 1);
    return float3(0.2f, 0.2f, 1.0f);
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