cbuffer cbPerObject
{
	matrix mtxWVP : WorldViewProjection;
};

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
};

VertexShaderOutput  VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = mul(float4(input.position, 1.0f), mtxWVP);

	//Notused-->
	//output.normal = input.normal;
	//output.tangent = input.tangent;
	//output.texCoord = input.texCoord;
	//<---

    return output;	
};

void PS(VertexShaderOutput input)
{
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
