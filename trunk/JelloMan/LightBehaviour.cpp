#include "LightBehaviour.h"

#include "LightBehaviourNormal.h"
#include "LightBehaviourBroken.h"
#include "LightBehaviourRotator.h"

ISerializable* LightBehaviour::Create(DWORD id)
{
    switch (id)
    {
        case SerializeTypes::LightBehaviourNormal: return new LightBehaviourNormal();
        case SerializeTypes::LightBehaviourBroken: return new LightBehaviourBroken(BrokenLightType_Fluorescent);
        case SerializeTypes::LightBehaviourRotator: return new LightBehaviourRotator(Vector3::Up, 0.0f);
        default: ASSERT(false, "File corrupt"); return 0;
    }
}