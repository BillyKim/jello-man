struct PointLight
{
    float4 Color;

    float3 Position;
    float Multiplier;

    float AttenuationStart;
    float AttenuationEnd;
};
struct SpotLight
{
    float4 Color;

    float3 Position;

    float3 Direction;
    float Multiplier;

    float AttenuationStart;
    float AttenuationEnd;
    float Radius;
};