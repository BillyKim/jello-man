Texture2D normalSpecMap;
Texture2D positionGlossMap;
Texture2D randomNormalMap;
Texture2D backBuffer;

float randomSize;
float sampleRadius;
float intensity;
float scale;
float bias;

int bbWidth;
int bbHeight;
int minIterations;
int maxIterations;

matrix mtxWorldView;

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
	Filter = MIN_MAG_MIP_POINT;
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

float3 getPosition(in float2 uv)
{
	return mul(float4(positionGlossMap.Sample(mapSampler, uv).xyz, 1.0f), mtxWorldView).xyz;
}

float3 getNormal(in float2 uv)
{
	return normalSpecMap.Sample(mapSampler, uv).xyz;
}

float2 getRandom(in float2 uv)
{
	float2 screenSize = float2(bbWidth,bbHeight);

	return (randomNormalMap.Sample(mapSampler, screenSize * uv / randomSize).xy * 2.0f - 1.0f);
}

float doAmbientOcclusion(in float2 tcoord,in float2 uv, in float3 p, in float3 cnorm)
{
	float3 diff = getPosition(tcoord + uv) - p;
	const float3 v = normalize(diff);
	const float d = length(diff) * scale;
	return max(0.0, dot(cnorm, v) - bias) * (1.0 / (1.0 + d)) * intensity;
}

float3 PS_SSAO(VertexShaderOutput input) : SV_TARGET
{
	float3 bbCol = backBuffer.Sample(mapSampler, input.TexCoord).xyz;

	const float2 vec[4] = {	float2(1,0),float2(-1,0),
							float2(0,1),float2(0,-1) };

	float3 p = getPosition(input.TexCoord);
	float3 n = getNormal(input.TexCoord);
	float2 rand = getRandom(input.TexCoord);

	float ao = 0.0f;
	float rad = sampleRadius / p.z;

	//int iterations = minIterations;

	int iterations = lerp(maxIterations,minIterations,p.z/1000);  //LOD

	for (int j = 0; j < iterations; ++j)
	{
		float2 coord1 = reflect(vec[j], rand) * rad;
		float2 coord2 = float2(	coord1.x * 0.707 - coord1.y * 0.707,
								coord1.x * 0.707 + coord1.y * 0.707);
  
		ao += doAmbientOcclusion(input.TexCoord,coord1 * 0.25, p, n);
		ao += doAmbientOcclusion(input.TexCoord,coord2 * 0.5, p, n);
		ao += doAmbientOcclusion(input.TexCoord,coord1 * 0.75, p, n);
		ao += doAmbientOcclusion(input.TexCoord,coord2, p, n);
	}

	ao /= (float)iterations * 4.0;

	return (bbCol * (1.0f - ao));
}

technique10 tech_SSAO
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 VS();
        PixelShader = compile ps_4_0 PS_SSAO();
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
    }
}