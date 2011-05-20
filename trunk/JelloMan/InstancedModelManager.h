#pragma once
#include "stdafx.h"
#include "Model.h"
#include "Matrix.h"
#include "vertex.h"
#include <vector>
#include "RenderContext.h"
#include "DeferredPreEffectNormalsInstanced.h" 

namespace Instancing{

class InstancedModelManager
{
public:
	InstancedModelManager(ID3D10Device* pDxDevice);
	virtual ~InstancedModelManager(void);

	void SetModel(Model<VertexPosNormTanTex>* pModel);

	void AddInstance(const Matrix* mtxWorld);

	void Draw(RenderContext* pRendercontext);

	void SetDiffuse(Texture2D* tex) { m_pTexDiffuse = tex; }
    void SetSpec(Texture2D* tex) { m_pTexSpec = tex; }
    void SetGloss(Texture2D* tex) { m_pTexGloss = tex; }
    void SetNormal(Texture2D* tex) { m_pTexNormal = tex; }


private:
	UINT m_PrevCapacity;

	void UpdateBuffer();
	ID3D10Device* m_pDxDevice;

	vector<const Matrix*> m_mtxInstances;
	Model<VertexPosNormTanTex>* m_pModel;

	DeferredPreEffectNormalsInstanced* m_pEffect;

    Texture2D *m_pTexDiffuse, *m_pTexSpec, *m_pTexGloss, *m_pTexNormal; 

	vector<Matrix> m_CPUBuffer;
	ID3D10Buffer* m_pBuffer;
};

} // end namespace
