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
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
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
	float4 colorGlow : SV_TARGET0;
	float4 normalSpec : SV_TARGET1;
	float4 positionGloss : SV_TARGET2;
};
float3 CalcNormal(float3 normal, float3 tangent, float3 rgb)
{
	//NormalMap
	tangent = normalize(tangent);
	normal = normalize(normal);
	float dottn = dot(tangent,normal);
	tangent -= dottn * normal;
	float3 binormal = normalize(cross(tangent,normal));

	float3x3 assenstelsel = float3x3(binormal,tangent,normal);

	float3 xyz = float3(rgb.x * 2 - 1, rgb.y * 2 - 1, rgb.z * 2 - 1) ;

	return normalize(mul(float4(xyz, 0), assenstelsel));
}
PixelShaderOutput  PS(VertexShaderOutput input) 
{
	PixelShaderOutput output;

    float3 normal = CalcNormal(input.normal, input.tangent, normalMap.Sample(mapSampler, input.texCoord).rgb);

	output.colorGlow = float4(diffuseMap.Sample(mapSampler, input.texCoord).rgb, selected?1.0f:0.0);
    if (selected)
    {
        output.colorGlow.rgb = output.colorGlow.rgb * 0.5 + float3(1.0f, 1.0f, 0.0f) * 0.5;
    }
    output.colorGlow = saturate(output.colorGlow);

	output.normalSpec = float4(normal, specMap.Sample(mapSampler, input.texCoord).r);
	//output.normalSpec.xyz = input.normal;
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