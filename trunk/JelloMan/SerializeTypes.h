#pragma once
typedef unsigned long DWORD;

//Lights - 100
struct SerializeTypes
{
    const static DWORD SpotLight = 100;
    const static DWORD PointLight = 101;

    const static DWORD LightBehaviourNormal = 110;
    const static DWORD LightBehaviourBroken = 111;
    const static DWORD LightBehaviourRotator = 112;
};