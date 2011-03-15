cbuffer cbPerObject
{
	matrix mtxWVP : WorldViewProjection;
	matrix mtxWorld : World;
};

RasterizerState RState
{
	FillMode = Solid;
	CullMode = None;
};

struct VertexShaderInput
{
    float3 position : POSITION0;
	float3 normal : NORMAL;
	float2 texCoord: TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

VertexShaderOutput VS(VertexShaderInput input) 
{
    VertexShaderOutput output;

    output.position = mul(float4(input.position, 1.0f), mtxWVP);

	output.normal = mul(float4(input.normal, 0.0f), mtxWorld).xyz;

	output.texCoord = input.texCoord;

    return output;	
};

float4 PS(VertexShaderOutput input) : SV_TARGET
{	
	return float4(normalize(input.normal/2+0.5f), 1.0f);
};


technique10 tech1
{
	pass P0
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, PS() ));

		SetRasterizerState(RState);
	}
}