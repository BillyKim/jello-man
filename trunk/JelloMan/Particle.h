#pragma once
#include "Vector3.h"

struct Particle
{
	Vector3 position;
	Vector3 velocity;
	float density;
	float lifetime;
	NxU32 id;
	Vector3 collisionNormal;
};