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
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ZERO;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
};

struct VertexShaderInput
{
    float3 position : POSITION;
};

struct VertexShaderOutput
{
    float3 position : POSITION;
    //float depth : DEPTH;
};

VertexShaderOutput VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = input.position;

    return output;	
};
struct GS_OUT
{
    float4 pos : SV_POSITION;
};

float4 PS(GS_OUT input) : SV_TARGET
{	
    return float4(1.0, 1.0f, 1.0f, input.pos.w / 10000);
};


[maxvertexcount(4)]
void GS(point VertexShaderOutput input[1], inout TriangleStream<GS_OUT> triStream)
{
    float size = .2f;
    float4 v[4];
    v[0] = float4(-size, size*2, 0.0f, 0.0f);
    v[1] = float4(size, size*2, 0.0f, 0.0f);
    v[2] = float4(-size, -size*2, 0.0f, 0.0f);
    v[3] = float4(size, -size*2, 0.0f, 0.0f);

    GS_OUT gOut;
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        gOut.pos = v[i] + mul(float4(input[0].position, 1.0f), mtxWVP);
        triStream.Append(gOut);
    }
}

technique10 tech1
{
	pass P0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader( CompileShader ( gs_4_0, GS() ));
		SetPixelShader( CompileShader ( ps_4_0, PS() ));
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(rState);
	}
}