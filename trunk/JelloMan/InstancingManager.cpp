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


void InstancingManager::AddObject(IInstancible* obj)
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
        m_vecModelManagers.push_back(man);
	}

	ASSERT(man != 0, "Instancemanager == 0!");
	man->AddInstance(obj->GetMtxWorldPointer());
}
void InstancingManager::DeleteObject(IInstancible* obj)
{
	tstring id = obj->GetUniqueInstancingID();

    if (m_pAssetContainer->IsAssetPresent(id) == true)
    {
        InstancedModelManager* man = 0;
	    man = m_pAssetContainer->GetAsset(id);
        man->RemoveInstance(obj->GetMtxWorldPointer());
    }
    else        
	    PANIC("Trying to remove object used for instancing, but no such instancing is present");
}


void InstancingManager::Draw(RenderContext* pRenderContext)
{
    for_each(m_vecModelManagers.cbegin(), m_vecModelManagers.cend(), [&](InstancedModelManager* obj)
    {
        obj->Draw(pRenderContext);
    });
}
void InstancingManager::DrawShadow(RenderContext* pRenderContext)
{
    for_each(m_vecModelManagers.cbegin(), m_vecModelManagers.cend(), [&](InstancedModelManager* obj)
    {
        obj->DrawShadow(pRenderContext);
    });
}


void InstancingManager::Clear()
{
    m_vecModelManagers.clear();
    m_pAssetContainer->RemoveAllAssets();
}


} //end namespace