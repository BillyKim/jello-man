//******************************************************\\
//**************(c) 2011 AllNightGames******************\\
//******************************************************\\
//														\\
//		PostProcessor: EdgeDetection					\\
//														\\
//------------------------------------------------------\\

#include "blur.fx"

Texture2D backBuffer;
Texture2D normalMap;
Texture2D depthMap;
int bbWidth;
int bbHeight;

static const float mult1 = 1.0f;
static const float mult2 = 2.0f;

RasterizerState rState
{
	FillMode = Solid;
	ScissorEnable = false;
};

BlendState blend
{
	BlendEnable[0] = FALSE;
	SrcBlend = ONE;
	DestBlend = ZERO;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
};

SamplerState mapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

struct VertexShaderInput
{
    float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

VertexShaderOutput VS(VertexShaderInput input)
{
    VertexShaderOutput output;

    output.Position = input.Position;
    output.TexCoord = input.TexCoord;

    return output;
}

float grayscale(float3 color)
{
    return (color.r + color.b + color.g) / 3.0f;
}
float GetEdge(Texture2D map, float2 texCoord)
{
	float somY = 0, somX = 0;
	
	float2 ox = float2(1.0f / bbWidth, 0.0f);
	float2 oy = float2(0.0f, 1.0f / bbHeight);

	float c00 = grayscale(map.SampleLevel(mapSampler, texCoord - ox - oy, 0).rgb);
	float c01 = grayscale(map.SampleLevel(mapSampler, texCoord - oy, 0).rgb);
	float c02 = grayscale(map.SampleLevel(mapSampler, texCoord + ox - oy, 0).rgb);
	float c10 = grayscale(map.SampleLevel(mapSampler, texCoord - ox, 0).rgb);
	float c12 = grayscale(map.SampleLevel(mapSampler, texCoord + ox, 0).rgb);
	float c20 = grayscale(map.SampleLevel(mapSampler, texCoord - ox + oy, 0).rgb);
	float c21 = grayscale(map.SampleLevel(mapSampler, texCoord + oy, 0).rgb);
	float c22 = grayscale(map.SampleLevel(mapSampler, texCoord + ox + oy, 0).rgb);

	somX -= c00 * mult1;
    somX -= c01 * mult2;
    somX -= c02 * mult1;
    somX += c20 * mult1;
    somX += c21 * mult2;
    somX += c22 * mult1;

	somY -= c00 * mult1;
    somY += c02 * mult1;
    somY -= c10 * mult2;
    somY += c12 * mult2;
    somY -= c20 * mult1;
    somY += c22 * mult1;

	float endCol = somX * somX + somY * somY;
	return endCol;
}
float3 PSOverlay(VertexShaderOutput input) : SV_TARGET
{
    float edge = GetEdge(normalMap, input.TexCoord);
    edge += GetEdge(depthMap, input.TexCoord);
	
	float3 bbCol = backBuffer.Sample(mapSampler, input.TexCoord);
	float3 endCol = bbCol * (edge < 0.1f? 1.0f : 0.0f);

	return endCol;
}
float3 PSEdgeOnly(VertexShaderOutput input) : SV_TARGET
{
    float3 col = GetEdge(normalMap, input.TexCoord);
    col += GetEdge(depthMap, input.TexCoord);
	
	return col;
}
float3 PSAA(VertexShaderOutput input) : SV_TARGET
{
    float edge = GetEdge(normalMap, input.TexCoord);
    edge += GetEdge(depthMap, input.TexCoord);

	float3 bbCol = backBuffer.Sample(mapSampler, input.TexCoord);

	[branch]
	if (edge < 0.1f)
	{
		return bbCol;
	}
	else
	{
		float2 ox = float2(1.0f / bbWidth, 0.0f);
        float2 oy = float2(0.0f, 1.0f / bbHeight);

		float3 c00 = backBuffer.SampleLevel(mapSampler, input.TexCoord, 0);
        c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord - ox - oy, 0);
		c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord - oy, 0);
		c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord + ox - oy, 0);
		c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord - ox, 0 );
		c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord + ox, 0);
		c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord - ox + oy, 0);
		c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord + oy, 0);
		c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord + ox + oy, 0);

		c00 /= 9.0f;

		return c00;
	}
}

technique10 tech_Overlay
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 VS();
        PixelShader = compile ps_4_0 PSOverlay();
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
    }
}
technique10 tech_EdgeOnly
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 VS();
        PixelShader = compile ps_4_0 PSEdgeOnly();
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
    }
}
technique10 tech_AA
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 VS();
        PixelShader = compile ps_4_0 PSAA();
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
    }
}