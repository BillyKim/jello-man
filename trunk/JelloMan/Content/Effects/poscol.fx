cbuffer cbPerObject
{
	matrix mtxWVP : WorldViewProjection;
	float4 fColor : Color;
};

RasterizerState WireFrame
{
	FillMode = WIREFRAME;
	CullMode = NONE;
	AntialiasedLineEnable = true;
    MultisampleEnable = true;
};

BlendState blend
{
	BlendEnable[0] = FALSE;
};

struct VS_IN
{
	float3 pos		:POSITION;
};

struct VS_OUT
{
	float4 pos		:SV_POSITION;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	vOut.pos = mul(float4(vIn.pos, 1.0f), mtxWVP);
	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	return fColor;
}

technique10 Wires
{
    pass P0
    {
    	SetRasterizerState( WireFrame );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
    }
}
