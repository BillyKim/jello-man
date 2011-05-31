#include "InstancedModelManager.h"
#include "ContentManager.h"

namespace Instancing{

InstancedModelManager::InstancedModelManager(ID3D10Device* pDxDevice): m_pDxDevice(pDxDevice), m_pModel(0), m_pBuffer(0), m_PrevCapacity(0)
{
	m_pEffect = Content->LoadEffect<DeferredPreEffectNormalsInstanced>(_T("../Content/Effects/predeferredNormalInstanced.fx"));	
}


InstancedModelManager::~InstancedModelManager(void)
{
}

void InstancedModelManager::UpdateBuffer()
{
	if (m_pBuffer == 0 || m_CPUBuffer.capacity() != m_PrevCapacity)
	{
		SafeRelease(m_pBuffer);

		D3D10_BUFFER_DESC desc;
		desc.ByteWidth = sizeof(Matrix) * m_CPUBuffer.capacity();
		desc.Usage = D3D10_USAGE_DYNAMIC;
		desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		D3D10_SUBRESOURCE_DATA data;
		data.pSysMem = &m_CPUBuffer[0];

		m_pDxDevice->CreateBuffer(&desc, &data, &m_pBuffer);

		m_PrevCapacity = m_CPUBuffer.capacity();
	}
	else
    {
        void *pData;
		HR(m_pBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &pData));

        //To GPU
        memcpy(pData, static_cast<const void*>(m_CPUBuffer.data()), sizeof(Matrix) * m_CPUBuffer.size());

		m_pBuffer->Unmap();
    }

}
void InstancedModelManager::SetModel(Model<VertexPosNormTanTex>* pModel)
{
	m_pModel = pModel;
}

void InstancedModelManager::AddInstance(const Matrix* mtxWorld)
{
	m_mtxInstances.push_back(mtxWorld);
	m_CPUBuffer.push_back(*mtxWorld);
}
void InstancedModelManager::RemoveInstance(const Matrix* mtxWorld)
{
    m_mtxInstances.erase(remove(m_mtxInstances.begin(), m_mtxInstances.end(), mtxWorld));
    m_CPUBuffer.pop_back(); //make one smaller
}

void InstancedModelManager::Draw(RenderContext* pRendercontext)
{
	m_pEffect->SetDiffuseMap(m_pTexDiffuse);
	m_pEffect->SetSpecMap(m_pTexSpec);
	m_pEffect->SetGlossMap(m_pTexGloss);
	m_pEffect->SetNormalMap(m_pTexNormal);
	m_pEffect->SetViewProjection(pRendercontext->GetCamera()->GetViewProjection());

	//update buffer
	for (int i = 0; i < m_mtxInstances.size(); ++i)
	{
		m_CPUBuffer[i] = *m_mtxInstances[i];
	}
	UpdateBuffer();

	m_pModel->DrawInstanced(m_pEffect, m_pBuffer, m_mtxInstances.size());
}
void InstancedModelManager::DrawShadow(RenderContext* pRenderContext)
{
    pRenderContext->GetPreShadowEffectInstanced()->SetViewProjection(pRenderContext->GetCamera()->GetViewProjection());

	//update buffer

    //Use old buffer

	/*for (int i = 0; i < m_mtxInstances.size(); ++i)
	{
		m_CPUBuffer[i] = *m_mtxInstances[i];
	}
	UpdateBuffer();*/

	m_pModel->DrawInstanced(pRenderContext->GetPreShadowEffectInstanced(), m_pBuffer, m_mtxInstances.size());
}

} //end namespace