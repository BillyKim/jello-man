cbuffer cbPerObject
{
	matrix mtxWorld : World;
	matrix mtxWVP : WorldViewProjection;
	bool selected : Selected;
};

Texture2D diffuseMap : DiffuseMap;
Texture2D normalMap : NormalMap;
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
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

struct VertexShaderInput
{
    float3 position : POSITION0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord: TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3 tangent : TANGENT;
};

VertexShaderOutput  VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = mul(float4(input.position, 1.0f), mtxWVP);
	output.worldPos = mul(float4(input.position, 1.0f), mtxWorld).xyz;

	output.normal = mul(float4(input.normal, 0.0f), mtxWorld).xyz;
	output.tangent = mul(float4(input.tangent, 0.0f), mtxWorld).xyz;

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

	float3 normalize1 = normalize(input.normal);
	float3 normalize2 = normalize(input.tangent);
	float3 cross1 = cross(normalize1, normalize2);

	float3x3 basis1 = float3x3(cross1, normalize2, normalize1);

	float3 gNormalMap = 2.0f * normalMap.Sample(mapSampler, input.texCoord).rgb - 1.0f;

	float3 mtx1_tvec3 = mul(gNormalMap, basis1);
//	mtx1_tvec3.b *= -1;

	output.color = float4(diffuseMap.Sample(mapSampler, input.texCoord).rgb, 1.0f);
	if (selected)
		output.color += float4(1.0f,1.0f,0.0f,0.5f);

	output.normalSpec = float4(mtx1_tvec3, specMap.Sample(mapSampler, input.texCoord).r);
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