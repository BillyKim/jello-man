cbuffer cbPerObject
{
	matrix mtxWVP : WorldViewProjection;
};
Texture2D diffuseMap;

RasterizerState rState
{
	FillMode = Solid;
	ScissorEnable = false;
};

BlendState blend
{
	BlendEnable[0] = FALSE;
};

SamplerState mapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};


struct VS_IN
{
    float3 position : POSITION0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord: TEXCOORD0;
};

struct VS_OUT
{
	float4 pos	: SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	vOut.pos = mul(float4(vIn.position, 1.0f), mtxWVP);
    vOut.texCoord = vIn.texCoord;
    
	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	return diffuseMap.Sample(mapSampler, pIn.texCoord);
}

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
