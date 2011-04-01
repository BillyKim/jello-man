cbuffer cbPerObject
{
	matrix mtxWVP : WorldViewProjection;
	matrix mtxWorld : World;
};

RasterizerState DisableCulling { CullMode = NONE; };

RasterizerState WireFrame
{
	FillMode = WIREFRAME;
	CullMode = NONE;
	AntialiasedLineEnable = true;
    MultisampleEnable = true;
};

struct VS_IN
{
	float3 Pos		:POSITION;
	float3 Color	:COLOR;
};

struct VS_OUT
{
	float4 Pos		:SV_POSITION;
 	float4 Color	:COLOR;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	vOut.Pos = mul(float4(vIn.Pos, 1.0f), mtxWVP);
    vOut.Color = float4(vIn.Color,1);
	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	return pIn.Color;
}

technique10 Wires
{
    pass P0
    {
    	SetRasterizerState( WireFrame );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
