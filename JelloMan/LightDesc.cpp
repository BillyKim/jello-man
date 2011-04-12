#include "LightDesc.h"

SpotLightDesc::SpotLightDesc()
{
    ZeroMemory(this, sizeof(SpotLightDesc));
}

DirectionalLightDesc::DirectionalLightDesc()
{
    ZeroMemory(this, sizeof(DirectionalLightDesc));
}


PointLightDesc::PointLightDesc()
{
    ZeroMemory(this, sizeof(PointLightDesc));
}