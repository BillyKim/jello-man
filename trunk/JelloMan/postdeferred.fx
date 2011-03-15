cbuffer cbPerObject
{
	float3 vLightDir : LightDir;
	float3 cLightColor : LightColor;
	float3 vCamDir : CamDirection;
};

Texture2D colorMap : ColorMap;
Texture2D normalSpecMap : NormalSpecMap;
Texture2D positionGlossMap : PositionGlossMap;

SamplerState mapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
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

VertexShaderOutput  VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = input.position;

	output.texCoord = input.texCoord;

    return output;	
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};


float4  PS(PixelShaderInput input) : COLOR0
{
	float4 posGloss = positionGlossMap.Sample(mapSampler, input.texCoord);
	float4 col = colorMap.Sample(mapSampler, input.texCoord);
	float4 normalSpec = normalSpecMap.Sample(mapSampler, input.texCoord);

	//DiffuseLight
    float3 color = dot(normalize(normalSpec.rgb), normalize(vLightDir));

	//DiffuseColor
	color *= col.rgb;

	//Phong
	float y = saturate(dot(normalSpec.rgb, vLightDir));
	float3 reflect = normalize(normalSpec.rgb * y * 2 - vLightDir);
	float spec = saturate(dot(vCamDir, reflect));
	spec = pow(spec, posGloss.a * 25.0f);

	color += spec * cLightColor * normalSpec.a;

	color = saturate(color);
	color = float3(1.f, 0.f, 0.f);
    return float4(color, 1.0f);
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
