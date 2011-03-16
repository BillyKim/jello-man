#include "Light.h"

SpotLight::SpotLight()
{
    ZeroMemory(this, sizeof(SpotLight));
}

DirectionalLight::DirectionalLight()
{
    ZeroMemory(this, sizeof(DirectionalLight));
}


PointLight::PointLight()
{
    ZeroMemory(this, sizeof(PointLight));
}