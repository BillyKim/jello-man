#include "lights.fx"
#define MAXLIGHTS 64

cbuffer cbPerFrame
{
};

PointLight pointLights[MAXLIGHTS] : PointLightArr;
int pointLightCount : PointLightCount;

float3 vCamPos : CameraPosition;

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
	float3 vCamDir = normalize(vCamPos - posGloss.xyz);

	float3 normal = normalize(normalSpec.xyz);
	
	float3 endColor = float3(0, 0, 0);

	for (int i = 0; i < pointLightCount; ++i)
	{
		float3 vLightDir = pointLights[i].Position - posGloss.xyz;
		float dist = length(vLightDir);
		vLightDir /= dist;
		dist = max(dist, 1) / 10;

		//DiffuseLight
		float3 color = saturate(dot(normal, vLightDir)) * pointLights[i].Color;

		//DiffuseColor
		//color *= col.rgb;

		//Phong
		float y = saturate(dot(normal, vLightDir));
		float3 reflect = normalize(normal * y * 2 - vLightDir);
		float spec = saturate(dot(vCamDir, reflect));
		spec = pow(spec, posGloss.a * 1.0f);
		spec *= pointLights[i].Color * normalSpec.a;

		color += spec;

		color = color / (dist /* dist*/);

		endColor += color * pointLights[i].Multiplier;
	}

	endColor = saturate(endColor);

    return float4(endColor, 1);
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
