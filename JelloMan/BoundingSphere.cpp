#include "BoundingSphere.h"


BoundingSphere::BoundingSphere(void): position(0.0f, 0.0f, 0.0f), radius(0.0f)
{
}
BoundingSphere::BoundingSphere(Vector3 pos, float r) : position(pos), radius(r)
{
}


BoundingSphere::~BoundingSphere(void)
{
}
