#include "InstancingManager.h"
#include "IInstancible.h"
#include "ContentManager.h"

namespace Instancing{

InstancingManager::InstancingManager(ID3D10Device* pDxDevice): 
	m_pAssetContainer(new AssetContainer<InstancedModelManager>()), m_pDxDevice(pDxDevice)
{
}


InstancingManager::~InstancingManager(void)
{
	delete m_pAssetContainer;
}


void InstancingManager::AddLevelObject(IInstancible* obj)
{
	InstancedModelManager* man = 0;
	tstring id = obj->GetUniqueInstancingID();
	if (m_pAssetContainer->IsAssetPresent(id))
	{
		man = m_pAssetContainer->GetAsset(id);
	}
	else
	{
		man = new InstancedModelManager(m_pDxDevice);
		man->SetDiffuse(obj->GetDiffuse());
		man->SetSpec(obj->GetSpec());
		man->SetGloss(obj->GetGloss());
		man->SetNormal(obj->GetNormal());
		man->SetModel(obj->GetModel());
		m_pAssetContainer->AddAsset(id, man);
	}

	ASSERT(man != 0, "Instancemanager == 0!");
	man->AddInstance(obj->GetMtxWorldPointer());
}

} //end namespace