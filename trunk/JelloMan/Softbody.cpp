#include "Softbody.h"
#include "CookingStream.h"

Softbody::~Softbody(void)
{
}


void Softbody::InitSoftbody(PhysX* pPhysX, const tstring& path)
{
	ASSERT(m_pSoftbody == 0); //would be weird if the actor is already intialized

	m_pPhysX = pPhysX;
	
	//Make Actor
	NxSoftBodyDesc softbodyDesc;
	
	string str = string(path.begin(), path.end());
	NxSoftBodyMesh* pMesh = m_pPhysX->GetSDK()->createSoftBodyMesh(UserStream(str.c_str(), true));
	softbodyDesc.softBodyMesh = pMesh;
	
	softbodyDesc.globalPose = static_cast<NxMat34>(m_WorldMatrix);

	m_pSoftbody = m_pPhysX->GetScene()->createSoftBody(softbodyDesc);

    ASSERT(m_pSoftbody != 0);
}