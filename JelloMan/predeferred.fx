cbuffer cbPerObject
{
	matrix mtxWorld : World;
	matrix mtxWorldVP : WorldViewProjection;
};

Texture2D diffuseMap : DiffuseMap;
//Texture2D normalMap : NormalMap;
Texture2D specMap : SpecMap;
Texture2D glossMap : GlossMap;

sampler2D MapSampler
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

struct VertexShaderInput
{
    float4 position : POSITION0;
	float3 normal : NORMAL;
	float2 texCoord: TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 position : POSITION0;
	float3 worldPos : TEXCOORD1;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

VertexShaderOutput  VS(VS_INPUT input) 
{
    VertexShaderOutput output;

    output.position = mul(input.position, mtxWVP);
	output.worldPos = mul(input.position, mtxWorld).xyz;

	output.normal = mul(float4(input.normal, 0.0f), mtxWorld);
	//output.tangent = mul(float4(input.tangent, 0.0f), mtxWorld);
	//output.binormal = mul(float4(input.binormal, 0.0f), mtxWorld);

	output.texCoord = input.texCoord;

    return output;	
};

struct PixelShaderInput
{
    float4 position : POSITION0;
	float3 worldPos : TEXCOORD1;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct PixelShaderOutput
{
	float4 color : COLOR0;
	float4 normalSpec : COLOR1;
	float4 positionGloss : COLOR2;
};

PixelShaderOutput  PS(PS_INPUT input) 
{
	PixelShaderOutput output;
	output.color = float4(diffuseMap.Sample(mapSampler, input.texCoord), 1.0f);
	output.normalSpec = float4(input.normal, specMap.Sample(mapSampler, input.texCoord).r);
	output.positionGloss = float4(input.worldPos, glossMap.Sample(mapSampler, input.texCoord).r);
};


technique10 pass1
{
	pass one
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetPixelShader( CompileShader ( ps_4_0, PS() ));
		SetGeometryShader(NULL);
	}
}
