#include "LightBehaviourNormal.h"


LightBehaviourNormal::LightBehaviourNormal(void)
{
    //nothing to create
}


LightBehaviourNormal::~LightBehaviourNormal(void)
{
    //nothing to destroy
}

LightBehaviour* LightBehaviourNormal::Copy() const
{
    return new LightBehaviourNormal();
}

//Serialize
void LightBehaviourNormal::Serialize(Serializer*) const
{
}
void LightBehaviourNormal::Deserialize(Serializer*)
{
}