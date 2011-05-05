#pragma once
#include "D3DUtil.h"
#include <vector>
#include "vertex.h"
#include "Effect.h"

typedef D3D10_PRIMITIVE_TOPOLOGY TOPOLOGY_TYPE;

template<typename T>
class ModelMesh
{
public:
    ModelMesh(ID3D10Device* device, const tstring& name, TOPOLOGY_TYPE topoType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST) : 
			      m_pDevice(device)
			    , m_pIndexBuffer(0)
			    , m_pVertexBuffer(0)
                , m_Name(name)
				, m_TopologyType(topoType)
    {
    }
    ~ModelMesh(void)
    {
	    SafeRelease(m_pVertexBuffer);
	    SafeRelease(m_pIndexBuffer);
    }
   
    void SetIndices(vector<DWORD> indices)
    {
	    m_VecIndices = indices;
    
	    //if buffer exists => release
	    SafeRelease(m_pIndexBuffer);

	    D3D10_BUFFER_DESC ibd;
	    ibd.Usage = D3D10_USAGE_IMMUTABLE;
	    ibd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	    ibd.CPUAccessFlags = 0;
	    ibd.MiscFlags = 0;

	    D3D10_SUBRESOURCE_DATA initData;
	    initData.pSysMem = &m_VecIndices[0];

	    HR(m_pDevice->CreateBuffer(&ibd, &initData, &m_pIndexBuffer));
    }   
    void SetVertices(vector<T> vertices)
    {
	    m_VecVertices = vertices;

	    //if buffer exists => release
	    SafeRelease(m_pVertexBuffer);

	    D3D10_BUFFER_DESC bd;
	    bd.Usage = D3D10_USAGE_IMMUTABLE;
	    bd.ByteWidth = sizeof( T ) * m_VecVertices.size();
	    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	    bd.CPUAccessFlags = 0;
	    bd.MiscFlags = 0;

	    D3D10_SUBRESOURCE_DATA initData;
	    initData.pSysMem = &m_VecVertices[0];

	    HR(m_pDevice->CreateBuffer( &bd, &initData, &m_pVertexBuffer ));
    }  
    const vector<T>& GetVertices()
    {
        return m_VecVertices;
    }

    Effect* GetEffect() const
    {
	    return m_pEffect;
    }  
    void SetIA(Effect* effect) const
    {
        ASSERT(m_pVertexBuffer != 0 && m_pIndexBuffer != 0 && effect != 0, "");

	    m_pDevice->IASetInputLayout(effect->GetInputLayout());

        // Set vertex buffer(s)
        UINT offset = 0;
        UINT vertexStride = effect->GetVertexStride();
        m_pDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &vertexStride, &offset);
   	
	    // Set index buffer
	    m_pDevice->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // Set primitive topology
        m_pDevice->IASetPrimitiveTopology(m_TopologyType);
    }
    void Draw(Effect* effect) const
    {
        SetIA(effect);       

        for(UINT p = 0; p < effect->GetNumPasses(); ++p)
        {
            effect->GetCurrentTechnique()->GetPassByIndex(p)->Apply(0);
			m_pDevice->DrawIndexed(m_VecIndices.size(), 0, 0); 
        }
    }

private:
    ID3D10Device* m_pDevice;

	ID3D10Buffer* m_pVertexBuffer;
	ID3D10Buffer* m_pIndexBuffer;

	vector<T> m_VecVertices;
	vector<DWORD> m_VecIndices;

    tstring m_Name;

	TOPOLOGY_TYPE m_TopologyType;
};

