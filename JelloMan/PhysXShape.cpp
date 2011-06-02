#include "PhysXShape.h"

#include "PhysXBox.h"
#include "PhysXSphere.h"
#include "PhysXMesh.h"

ISerializable* PhysXShape::GetShape(DWORD type)
{
    switch (type)
    {
        case SerializeTypes::PhysXShapeBox: return new PhysXBox();
        case SerializeTypes::PhysXShapeSphere: return new PhysXSphere();
        case SerializeTypes::PhysXShapeMesh: return new PhysXMesh();
        default: PANIC("Shape not recognized"); return 0;
    }
}