const static float mtxGauss[5][5] =
   {{0.00078633f,	0.00655965f,    0.01330373f,	0.00655965f,    0.00078633f},
    {0.00655965f,   0.05472157f,	0.11098164f,    0.05472157f,	0.00655965f},
    {0.01330373f,	0.11098164f,    0.22508352f,    0.11098164f,    0.01330373f},
    {0.00655965f,   0.05472157f,	0.11098164f,    0.05472157f,	0.00655965f},
    {0.00078633f,	0.00655965f,    0.01330373f,	0.00655965f,    0.00078633f}};

SamplerState blurSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

float4 gaussblur(in Texture2D tex, in float2 texCoord, in float2 t0, float depth)
{
    float4 col = float4(0, 0, 0, 0);

    float blur = 15 * (1-depth) + 2.0f;

    [unroll]
    for (int x = -2; x <= 2; x += 1)
        [unroll]
        for (int y = -2; y <= 2; y += 1)
            col += tex.Sample(blurSampler, texCoord + t0 * float2(x * blur, y * blur)) * mtxGauss[x+2][y+2];

    return col;
}
float4 boxblur(in Texture2D tex, in float2 texCoord, in float2 t0, float depth)
{
    float4 col = float4(0, 0, 0, 0);

    float blur = 5 * (1-depth)*(1-depth) + 2.0f;

    [unroll]
    for (int x = -2; x <= 2; x += 1)
        [unroll]
        for (int y = -2; y <= 2; y += 1)
            col += tex.Sample(blurSampler, texCoord + t0 * float2(x * blur, y * blur));

    return col /= 25.0f;
}