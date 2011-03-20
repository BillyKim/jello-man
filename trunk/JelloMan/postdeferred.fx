#include "lights.fx"

cbuffer cbPerFrame
{
};

PointLight pointLight : PointLight;

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

float3  PS(VertexShaderOutput input) : SV_TARGET
{
	float4 col = colorMap.Sample(mapSampler, input.texCoord);
	if (col.r == 0.0f && col.g == 0.0f && col.b == 0.0f)
		return col.xyz;

	float4 posGloss = positionGlossMap.Sample(mapSampler, input.texCoord);
	float4 normalSpec = normalSpecMap.Sample(mapSampler, input.texCoord);
	float3 vCamDir = normalize(vCamPos - posGloss.xyz);

	float3 normal = normalize(normalSpec.xyz);
	
	float3 endColor = float3(0, 0, 0);


	float3 vLightDir = pointLight.Position - posGloss.xyz;
	float dist = length(vLightDir);

	[branch]
	if (dist < pointLight.AttenuationEnd)
	{
		vLightDir /= dist;

		//DiffuseLight
		float diff = dot(normal, vLightDir);
			
		[branch]
		if (diff > 0)
		{
			float3 color = diff * pointLight.Color.rgb;

			//DiffuseColor
			color *= col.rgb;

			//Phong
			float y = max(dot(normal, vLightDir), 0);
			float3 reflect = normalize(normal * y * 2 - vLightDir);
			float spec = saturate(dot(vCamDir, reflect));
			spec = pow(spec, posGloss.a * 25.0f);
			spec *= normalSpec.a;

			color += spec * pointLight.Color.rgb;

			color *= 1 - (max(dist - pointLight.AttenuationStart, 0) / (pointLight.AttenuationEnd - pointLight.AttenuationStart));
			//color = color / dist;

			endColor += color * pointLight.Multiplier;
		}
	}

	endColor = saturate(endColor);

	//endColor = normalSpec.xyz;

    return endColor;
};


technique10 tech1
{
	pass p0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS() ));
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
	}
}

technique10 tech2
{
	pass p0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS_UNLIT() ));
	}
}