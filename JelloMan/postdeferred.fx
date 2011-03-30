#include "lights.fx"

cbuffer cbPerFrame
{
};

PointLight pointLight : PointLight;
SpotLight spotLight : SpotLight;

float3 vCamPos : CameraPosition;

Texture2D colorMap : ColorMap;
Texture2D normalSpecMap : NormalSpecMap;
Texture2D positionGlossMap : PositionGlossMap;

BlendState blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
};

RasterizerState rState
{
	FillMode = Solid;
	ScissorEnable = true;
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
    float4 position : POSITION0;
	float2 texCoord: TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

VertexShaderOutput  VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = input.position;

	output.texCoord = input.texCoord;

    return output;	
};

float3  PS_UNLIT(VertexShaderOutput input) : SV_TARGET
{
	float4 col = colorMap.Sample(mapSampler, input.texCoord);
	float3 endColor = float3(col.r,col.g,col.b);

	return endColor;
}

float PointShadowCheck()
{
	return 1.0f;
}
float SpotShadowCheck()
{
	return 1.0f;
}
float3  PS_Point(VertexShaderOutput input)
{
	//Check if light is not too far from pixel
	//else clip
	float4 posGloss = positionGlossMap.Sample(mapSampler, input.texCoord);
	float3 vLightDir = pointLight.Position - posGloss.xyz;
	float dist = length(vLightDir);

	clip(dist >= pointLight.AttenuationEnd? -1 : 1);

	//calc diffuse light if <= 0 clip
	//DiffuseLight
	vLightDir /= dist; //normalize
	float4 normalSpec = normalSpecMap.Sample(mapSampler, input.texCoord);
	float3 normal = normalize(normalSpec.xyz);
	float diff = sqrt(dot(normal, vLightDir) * 0.5f + 0.5f);
				
	clip(diff <= 0? -1 : 1);

	//DiffuseColor
	float3 color = diff * pointLight.Color.rgb;
	float4 col = colorMap.Sample(mapSampler, input.texCoord);
	color *= col.rgb;


	//Phong
	float3 vCamDir = normalize(vCamPos - posGloss.xyz);

	float y = max(dot(normal, vLightDir), 0);
	float spec;
	[branch]
	if (y > 0)
	{
		float3 reflect = normalize(normal * y * 2 - vLightDir);
		spec = saturate(dot(vCamDir, reflect));
		spec = pow(spec, posGloss.a * 25.0f);
		spec *= normalSpec.a;
	}
	else
		spec = 0;

	color += spec * pointLight.Color.rgb;

	//attenuation
	color *= 1 - (max(dist - pointLight.AttenuationStart, 0) / (pointLight.AttenuationEnd - pointLight.AttenuationStart));

	color *= pointLight.Multiplier;

	color = saturate(color);

    return color;
};
float3  PS_Spot(VertexShaderOutput input)
{
	//Check if light is not too far from pixel
	//else clip
	float4 posGloss = positionGlossMap.Sample(mapSampler, input.texCoord);
	float3 vLightDir = spotLight.Position - posGloss.xyz;
	float dist = length(vLightDir);

	clip(dist >= spotLight.AttenuationEnd? -1 : 1);

	//calc diffuse light if <= 0 clip
	//DiffuseLight
	vLightDir /= dist; //normalize
	float4 normalSpec = normalSpecMap.Sample(mapSampler, input.texCoord);
	float3 normal = normalize(normalSpec.xyz);

	
	float s = dot(-vLightDir, spotLight.Direction);
	s = pow(s, spotLight.Radius);
	clip(s <= 0.01f? -1 : 1); //clip if <= 0.01f

	float diff = dot(normal, vLightDir);
				
	clip(diff <= 0? -1 : 1);

	//DiffuseColor
	float3 color = diff * spotLight.Color.rgb;
	float4 col = colorMap.Sample(mapSampler, input.texCoord);
	color *= col.rgb;

	//Phong
	float3 vCamDir = normalize(vCamPos - posGloss.xyz);

	float y = max(dot(normal, vLightDir), 0);
	float3 reflect = normalize(normal * y * 2 - vLightDir);
	float spec = saturate(dot(vCamDir, reflect));
	spec = pow(spec, posGloss.a * 100.0f);
	spec *= normalSpec.a;

	color += spec * spotLight.Color.rgb;

	//attenuation
	color *= 1 - (max(dist - spotLight.AttenuationStart, 0) / (spotLight.AttenuationEnd - spotLight.AttenuationStart));

	color *= spotLight.Multiplier * s;

	color = saturate(color);

    return color;
}

float3 PS_PointNoShadows(VertexShaderOutput input) : SV_TARGET
{
	return PS_Point(input);
}
float3 PS_SpotNoShadows(VertexShaderOutput input) : SV_TARGET
{
	return PS_Spot(input);
}
float3 PS_SpotShadows(VertexShaderOutput input) : SV_TARGET
{
	float shadowMult = SpotShadowCheck();
	clip (shadowMult < 0.01f? -1 : 1);

	return PS_Spot(input) * shadowMult;
}


technique10 tech_PointLightNoShadows
{
	pass p0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS_PointNoShadows() ));

		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
	}
}
technique10 tech_SpotLightNoShadows
{
	pass p0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS_SpotNoShadows() ));

		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
	}
}
technique10 tech_SpotLightShadows
{
	pass p0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS_SpotShadows() ));

		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
	}
}
technique10 tech_UNLIT
{
	pass p0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS_UNLIT() ));
	}
}