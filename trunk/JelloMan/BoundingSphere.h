#pragma once

#include "Vector3.h"

struct BoundingSphere
{
public:
    BoundingSphere(void);
    BoundingSphere(Vector3 pos, float r);
    ~BoundingSphere(void);

    //Default copy and assignment is fine

    Vector3 position;
    float radius;
};

