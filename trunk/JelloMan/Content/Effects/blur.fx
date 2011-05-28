const static float mtxGauss[7][7] =
{{0.00000067f,	0.00002292f,	0.00019117f,	0.00038771f,	0.00019117f,	0.00002292f,	0.00000067f},
 {0.00002292f,	0.00078633f,	0.00655965f,    0.01330373f,	0.00655965f,    0.00078633f,	0.00002292f},
 {0.00019117f,	0.00655965f,    0.05472157f,	0.11098164f,    0.05472157f,	0.00655965f,    0.00019117f},
 {0.00038771f,	0.01330373f,	0.11098164f,    0.22508352f,    0.11098164f,    0.01330373f,	0.00038771f},
 {0.00019117f,	0.00655965f,    0.05472157f,	0.11098164f,    0.05472157f,	0.00655965f,    0.00019117f},
 {0.00002292f,	0.00078633f,	0.00655965f,    0.01330373f,	0.00655965f,    0.00078633f,	0.00002292f},
 {0.00000067f,	0.00002292f,	0.00019117f,	0.00038771f,	0.00019117f,	0.00002292f,	0.00000067f}};

SamplerState blurSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

float4 gaussblur(in Texture2D tex, in float2 texCoord, in float2 t0)
{
    float4 col = float4(0, 0, 0, 0);

    float blur = 5;

    [unroll]
    for (int x = -3; x <= 3; x += 1)
        [unroll]
        for (int y = -3; y <= 3; y += 1)
            col += tex.Sample(blurSampler, texCoord + t0 * float2(x * blur, y * blur)) * mtxGauss[x+3][y+3];

    return col;
}
float4 boxblur(in Texture2D tex, in float2 texCoord, in float2 t0)
{
    float4 col = float4(0, 0, 0, 0);

    float blur = 4;

    [unroll]
    for (int x = -8; x <= 8; x += 1)
        [unroll]
        for (int y = -8; y <= 8; y += 1)
            col += tex.Sample(blurSampler, texCoord + t0 * float2(x * blur, y * blur));

    return col /= 9*9;
}