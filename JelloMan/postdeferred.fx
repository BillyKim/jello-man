cbuffer cbPerFrame
{
	float3 vLightDir : LightDir;
	float3 cLightColor : LightColor;
	float3 vCamPos : CamPosition;
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
    float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

RasterizerState RState
{
	FillMode = Solid;
	CullMode = None;
};

VertexShaderOutput  VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = input.position;

	output.texCoord = input.texCoord;

    return output;	
};

float4  PS(VertexShaderOutput input) : SV_TARGET
{
	float4 posGloss = positionGlossMap.Sample(mapSampler, input.texCoord);
	float4 col = colorMap.Sample(mapSampler, input.texCoord);
	float4 normalSpec = normalSpecMap.Sample(mapSampler, input.texCoord);

	//DiffuseLight
    float3 color = dot(normalize(normalSpec.rgb), normalize(vLightDir));

	//DiffuseColor
	color *= col.rgb;
	//float3 color = col.rgb;

	float3 vCamDir = normalize(vCamPos - posGloss.xyz);

	//Phong
	float y = saturate(dot(normalSpec.rgb, vLightDir));
	float3 reflect = normalize(normalSpec.rgb * y * 2 - vLightDir);
	float spec = saturate(dot(vCamDir, reflect));
	spec = pow(spec, posGloss.a * 25.0f);

	color += spec * cLightColor * normalSpec.a;

	color = saturate(color);

	//return float3(1, 0, 0);
    return float4(color, 1);
};


technique10 tech1
{
	pass p0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS() ));

		SetRasterizerState(RState);
	}
}
