//******************************************************\\
//**************(c) 2011 AllNightGames******************\\
//******************************************************\\
//														\\
//		PostProcessor: EdgeDetection					\\
//														\\
//------------------------------------------------------\\

Texture2D backBuffer;
int bbWidth;
int bbHeight;

static const float mult1 = 3.0f;
static const float mult2 = 10.0f;

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


float3 GetEdge(float2 texCoord)
{
	float3 somY = 0, somX = 0;
	
	float2 ox = float2(1.0f / bbWidth, 0.0f);
	float2 oy = float2(0.0f, 1.0f / bbHeight);

	float3 c00 = backBuffer.SampleLevel(mapSampler, texCoord - ox - oy, 0);
	float3 c01 = backBuffer.SampleLevel(mapSampler, texCoord - oy, 0);
	float3 c02 = backBuffer.SampleLevel(mapSampler, texCoord + ox - oy, 0);
	float3 c10 = backBuffer.SampleLevel(mapSampler, texCoord - ox, 0);
	float3 c12 = backBuffer.SampleLevel(mapSampler, texCoord + ox, 0);
	float3 c20 = backBuffer.SampleLevel(mapSampler, texCoord - ox + oy, 0);
	float3 c21 = backBuffer.SampleLevel(mapSampler, texCoord + oy, 0);
	float3 c22 = backBuffer.SampleLevel(mapSampler, texCoord + ox + oy, 0);


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

	float3 endCol = somX * somX + somY * somY;
	return 1-endCol;
}
float3 PSOverlay(VertexShaderOutput input) : SV_TARGET
{
    float3 col = GetEdge(input.TexCoord);
	
	float3 bbCol = backBuffer.Sample(mapSampler, input.TexCoord);
	float3 endCol;

	if (bbCol.r > 0.5f) 
		endCol.r = 1.0f - (1.0f - 2.0f * (bbCol.r - 0.5f)) * (1.0f - col.r);
	else 
		endCol.r = (2.0f * bbCol.r) * col.r; 
	if (bbCol.g > 0.5f) 
		endCol.g = 1.0f - (1.0f - 2.0f * (bbCol.g - 0.5f)) * (1.0f - col.g);
	else 
		endCol.g = (2.0f * bbCol.g) * col.g; 
	if (bbCol.b > 0.5f) 
		endCol.b = 1.0f - (1.0f - 2.0f * (bbCol.b - 0.5f)) * (1.0f - col.b);
	else 
		endCol.b = (2.0f * bbCol.b) * col.b; 

    return endCol;
}
float3 PSEdgeOnly(VertexShaderOutput input) : SV_TARGET
{
	return GetEdge(input.TexCoord);
}
float3 PSAA(VertexShaderOutput input) : SV_TARGET
{
	float3 g_edge = GetEdge(input.TexCoord);
	float edge = (g_edge.x + g_edge.y + g_edge.z) / 3.0f;

	float3 bbCol = backBuffer.Sample(mapSampler, input.TexCoord);

	[branch]
	if (edge > 0.99f)
	{
		return bbCol;
	}
	else
	{
		float2 ox = float2(1.0f / bbWidth, 0.0f);
		float2 oy = float2(0.0f, 1.0f / bbHeight);

		float3 c00 = backBuffer.SampleLevel(mapSampler, input.TexCoord - ox - oy, 0);
		 c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord - oy, 0);
		 c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord + ox - oy, 0);
		 c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord - ox, 0 );
		 c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord + ox, 0);
		 c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord - ox + oy, 0);
		 c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord + oy, 0);
		 c00 += backBuffer.SampleLevel(mapSampler, input.TexCoord + ox + oy, 0);

		 c00 /= 8.0f;

		 return c00;
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