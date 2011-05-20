cbuffer cbPerObject
{
	matrix mtxWorld : World;
	matrix mtxWVP : WorldViewProjection;
	bool selected : Selected;
};

Texture2D diffuseMap : DiffuseMap;
Texture2D specMap : SpecMap;
Texture2D glossMap : GlossMap;

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
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

struct VertexShaderInput
{
    float3 position : POSITION0;
	float3 normal : NORMAL;
	float2 texCoord: TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
};

VertexShaderOutput  VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = mul(float4(input.position, 1.0f), mtxWVP);
	output.worldPos = mul(float4(input.position, 1.0f), mtxWorld).xyz;

	output.normal = mul(float4(input.normal, 0.0f), mtxWorld).xyz;

	output.texCoord = input.texCoord;

    return output;	
};

struct PixelShaderOutput
{
	float4 color : SV_TARGET0;
	float4 normalSpec : SV_TARGET1;
	float4 positionGloss : SV_TARGET2;
};

PixelShaderOutput  PS(VertexShaderOutput input) 
{
	PixelShaderOutput output;
	output.color = float4(diffuseMap.Sample(mapSampler, input.texCoord).rgb, 1.0f);

	if (selected)
		output.color += float4(1.0f,1.0f,0.0f,0.5f);

	output.normalSpec = float4(input.normal, specMap.Sample(mapSampler, input.texCoord).r);
	output.positionGloss = float4(input.worldPos, glossMap.Sample(mapSampler, input.texCoord).r);

	return output;
};


technique10 tech1
{
	pass P0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS() ));
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
	}
}
