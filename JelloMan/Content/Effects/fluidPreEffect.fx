cbuffer cbPerFrame
{
	matrix mtxWVP : WorldViewProjection;
    float3 camPos;
};

Texture2D texRainbow;

SamplerState mapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

RasterizerState rState
{
	FillMode = Solid;
	ScissorEnable = false;
};

BlendState blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
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
    float3 worldCenter: TEXCOORD1;
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
        gOut.worldCenter = input[0].position;
        triStream.Append(gOut);
    }
}
struct PSOut
{
    float4 colorMap: SV_TARGET0;
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

    normal.z = cross(float3(normal.x, 0, 0), float3(0, normal.y, 0));
    normal = normalize(normal);

    float3 vCamDir = normalize(camPos - input.worldCenter);
    float2 tex = (float2(dot(vCamDir, normal), 0.0f) + 1) / 2;

    psOut.colorMap = float4(texRainbow.Sample(mapSampler, tex).rgb, r2 * r2);

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