#include "TestPhysXBox.h"
#include "PhysXBox.h"

TestPhysXBox::TestPhysXBox(PhysX* pPhysX, const Vector3& pos) : Actor()
{
    PhysXBox box = PhysXBox(Vector3(1, 1, 1), 1);
    InitActor(pPhysX, box, true);
    Translate(pos);
}


TestPhysXBox::~TestPhysXBox(void)
{

}
