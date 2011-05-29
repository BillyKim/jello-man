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
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
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
    float2 texCoord: TEXCOORD0;
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

    float2 t[4];
    t[0] = float2(0, 1);
    t[1] = float2(1, 1);
    t[2] = float2(0, 0);
    t[3] = float2(1, 0);

    GS_OUT gOut;
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        gOut.pos = mul(float4(input[0].position, 1.0f), mtxWVP) + v[i];
        gOut.texCoord = t[i];
        triStream.Append(gOut);
    }
}
struct PSOut
{
    float4 normalMap: SV_TARGET0;
    //float4 depthMap: SV_TARGET1;
};
PSOut PS(GS_OUT input)
{	
    PSOut psOut;

    // calculate eye-space sphere normal from texture coordinates
    float3 normal;
    normal.xy = input.texCoord * 2.0 - 1.0;

    // kill pixels outside circle
    float r2 = dot(normal.xy, normal.xy);
    if (r2 > 1.0) discard;

    normal.z = 1.0f;

    normal = normalize(normal);

    // calculate depth
    //float4 pixelPos = float4(vCamPos + normal * sphereRadius, 1.0);
    //float4 clipSpacePos = mul(pixelPos, mtxWVP);
    //out.depthMap = clipSpacePos.z / clipSpacePos.w;
    psOut.normalMap = float4((normal + 1) / 2.0f, 1.0f);

    return psOut;
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