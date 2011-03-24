struct PointLight
{
    float4 Color;

    float3 Position;
    float Multiplier;

    float AttenuationStart;
    float AttenuationEnd;

    //bool Enabled;
};
struct SpotLight
{
    float4 Color;

    float3 Position;
    float Multiplier;

    float AttenuationStart;
    float AttenuationEnd;
    float Radius;

    float3 Direction;

    //bool Enabled;
};