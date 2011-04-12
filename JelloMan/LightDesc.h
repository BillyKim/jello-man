#pragma once
#include "Color.h"

////-------SpotLight--------------------->
struct SpotLightDesc
{
	SpotLightDesc();

    Color color;

    Vector3 position;
    float padding1;

    Vector3 direction;
    float multiplier;

    float attenuationStart;
    float attenuationEnd;
    float power;
    float padding2;
};
//<-------------------------------------

//-------DirectionalLight--------------------->
struct DirectionalLightDesc
{
	DirectionalLightDesc();

    Color color;
    Vector3 position;
    float multiplier;
    
    Vector3 direction;
    float padding2;
};
//<--------------------------------------------

//-------PointLight--------------------->
struct PointLightDesc
{
	PointLightDesc();

    Color color;

    Vector3 position;
    float multiplier;

    float attenuationStart;
    float attenuationEnd;
	Vector2 padding;
};
//<--------------------------------------------

