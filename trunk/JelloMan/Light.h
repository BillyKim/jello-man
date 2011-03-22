#pragma once
#include "Color.h"

////-------SpotLight--------------------->
struct SpotLight
{
    SpotLight();

    Color color;
    Vector3 position;
    float multiplier;

    float AttenuationStart;
    float AttenuationEnd;
    float radius;
    float padding;

    Vector3 direction;
    float padding2;
    //bool m_Enabled;
};
//<-------------------------------------

//-------DirectionalLight--------------------->
struct DirectionalLight
{
    DirectionalLight();

    Color color;
    Vector3 position;
    float multiplier;
    
    Vector3 direction;
    float padding2;
    //bool m_Enabled;
};
//<--------------------------------------------

//-------PointLight--------------------->
struct PointLight
{
    PointLight();

    Color color;
    Vector3 position;
    float multiplier;

    float AttenuationStart;
    float AttenuationEnd;
	Vector2 padding;

    //bool m_Enabled;
};
//<--------------------------------------------

