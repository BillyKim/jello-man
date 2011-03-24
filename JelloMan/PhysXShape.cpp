#include "PhysXShape.h"


PhysXShape::PhysXShape(void): m_Shape(0)
{

}


PhysXShape::~PhysXShape(void)
{
	delete m_Shape;
}