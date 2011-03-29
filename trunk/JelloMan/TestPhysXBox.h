#pragma once
#include "Actor.h"

class TestPhysXBox : public Actor
{
public:
    TestPhysXBox(PhysX* pPhysX, const Vector3& pos);
    virtual ~TestPhysXBox(void);
};

