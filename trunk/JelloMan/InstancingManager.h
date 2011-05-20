#pragma once
#include "AssetContainer.h"
#include "InstancedModelManager.h"
#include "ILevelObject.h"
#include "IInstancible.h"

namespace Instancing{

class InstancingManager
{
public:
	InstancingManager(ID3D10Device* pDxDevice);
	virtual ~InstancingManager(void);

	void AddLevelObject(IInstancible* obj);

private:
	AssetContainer<InstancedModelManager>* m_pAssetContainer;

	ID3D10Device* m_pDxDevice;
};

} //end namespace
