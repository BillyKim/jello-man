#pragma once
#include "AssetContainer.h"
#include "InstancedModelManager.h"
#include "ILevelObject.h"
#include "IInstancible.h"
#include "RenderContext.h"

namespace Instancing{

class InstancingManager
{
public:
	InstancingManager(ID3D10Device* pDxDevice);
	virtual ~InstancingManager(void);

	void AddLevelObject(IInstancible* obj);

    void Draw(RenderContext* pRenderContext);

private:
	AssetContainer<InstancedModelManager>* m_pAssetContainer;

    vector<InstancedModelManager*> m_vecModelManagers;

	ID3D10Device* m_pDxDevice;
};

} //end namespace
