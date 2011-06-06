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

    const static DWORD PhysXShapeBox = 500;
    const static DWORD PhysXShapeSphere = 501;
    const static DWORD PhysXShapeMesh = 502;

    const static DWORD PhysXTrigger = 600;

    const static DWORD SimpleObject = 1000;
    const static DWORD SpawnPoint = 1100;
};