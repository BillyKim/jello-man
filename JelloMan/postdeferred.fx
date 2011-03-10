cbuffer cbPerObject
{
	matrix mtxWorld : World;
	matrix mtxWorldVP : WorldViewProjection;

	float3 vLightDir : LightDir;
	float3 cLightColor : LightColor;
	floar3 vCamDir : CamDirection
};

Texture2D colorMap : ColorMap;
Texture2D normalSpecMap : NormalSpecMap;
Texture2D positionGlossMap : PositionGlossMap;

sampler2D MapSampler
{
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

struct VertexShaderInput
{
    float4 position : POSITION0;
	float2 texCoord: TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
};

VertexShaderOutput  VS(VS_INPUT input) 
{
    VertexShaderOutput output;

    output.position = mul(input.position, mtxWVP);

	output.texCoord = input.texCoord;

    return output;	
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

struct PixelShaderOutput
{
	float4 color : COLOR0;
};

PixelShaderOutput  PS(PS_INPUT input) 
{
	float4 posGloss = positionGlossMap.Sample(mapSampler, input.texCoord);
	float4 col = colorMap.Sample(mapSampler, input.texCoord);
	float4 normalSpec = normalSpecMap.Sample(mapSampler, input.texCoord);

	//DiffuseLight
    float3 color = dot(normalize(input.normal), normalize(vLightDir));

	//DiffuseColor
	color *= col.rgb;

	//Phong
	float y = saturate(dot(normalSpec.rgb, vLightDir));
	float3 reflect = normalize(normalSpec.rgb * y * 2 - vLightDir);
	float spec = saturate(dot(vCamDir, reflect));
	spec = pow(spec, posGloss.a * 25.0f);
	spec *= cLightColor * normalSpec.a;

	color += spec;

	color = saturate(color);

    return float4(color, texSample.a);
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
