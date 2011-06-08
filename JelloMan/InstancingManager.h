#pragma once
#include "AssetContainer.h"
#include "InstancedModelManager.h"
#include "ILevelObject.h"
#include "IInstancible.h"
#include "RenderContext.h"
#include "PreShadowEffectInstanced.h"

namespace Instancing{

class InstancingManager
{
public:
	InstancingManager(ID3D10Device* pDxDevice);
	virtual ~InstancingManager(void);

    void Clear();
	void AddObject(IInstancible* obj);
	void DeleteObject(IInstancible* obj);

    void Draw(const RenderContext* pRenderContext);
    void DrawShadow(const RenderContext* pRenderContext);

private:
	AssetContainer<InstancedModelManager>* m_pAssetContainer;

    vector<InstancedModelManager*> m_vecModelManagers;

	ID3D10Device* m_pDxDevice;

	//Disable default copy constructor and assignment operator
    InstancingManager(const InstancingManager& copy);
    InstancingManager& operator=(const InstancingManager& other);
};

} //end namespace
