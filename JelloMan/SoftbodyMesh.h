#pragma once
#include "D3DUtil.h"
#include <vector>
#include "vertex.h"
#include "Effect.h"

class SoftbodyMesh
{
public:
    SoftbodyMesh(ID3D10Device* device, const tstring& name) : 
			      m_pDevice(device)
			    , m_pIndexBuffer(0)
			    , m_pVertexBuffer(0)
			    , m_pEffect(0)
	            , m_pInputLayout(0)
	            , m_VertexBufferStride(0)
                , m_Name(name)
    {
    }
    ~SoftbodyMesh(void)
    {
	    SafeRelease(m_pVertexBuffer);
	    SafeRelease(m_pIndexBuffer);
        SafeRelease(m_pInputLayout);
    }
   
    void SetIndices(const vector<DWORD>& indices)
    {
	    m_VecIndices = indices;
    
	    //if buffer exists => release
	    SafeRelease(m_pIndexBuffer);

	    D3D10_BUFFER_DESC ibd;
	    ibd.Usage = D3D10_USAGE_IMMUTABLE;
	    ibd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	    ibd.CPUAccessFlags = 0;//D3D10_CPU_ACCESS_WRITE;
	    ibd.MiscFlags = 0;

	    D3D10_SUBRESOURCE_DATA initData;
	    initData.pSysMem = &m_VecIndices[0];

	    HR(m_pDevice->CreateBuffer(&ibd, &initData, &m_pIndexBuffer));
    }   
    void SetVertices(const vector<VertexPosNormTanTex>& vertices)
    {
	    if (m_pVertexBuffer == 0)
        {
	        m_VecVertices = vertices;

	        D3D10_BUFFER_DESC bd;
            bd.Usage = D3D10_USAGE_DYNAMIC;
	        bd.ByteWidth = sizeof( VertexPosNormTanTex ) * m_VecVertices.size();
	        bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	        bd.MiscFlags = 0;

	        D3D10_SUBRESOURCE_DATA initData;
	        initData.pSysMem = &m_VecVertices[0];

	        HR(m_pDevice->CreateBuffer( &bd, &initData, &m_pVertexBuffer ));
        }
        else
        {
            ASSERT(vertices.size() == m_VecVertices.size());

            void *pVertices;
            HR(m_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &pVertices));

            //To GPU
            memcpy(pVertices, static_cast<const void*>(vertices.data()), sizeof(VertexPosNormTanTex) * vertices.size());

            m_pVertexBuffer->Unmap();

            //To CPU
            memcpy(static_cast<void*>(m_VecVertices.data()), static_cast<const void*>(vertices.data()), sizeof(VertexPosNormTanTex) * vertices.size());

        }
    }  
    void SetTetra(const vector<DWORD>& tetra)
    {
        m_VecTetra = tetra;
    }
    void SetBaryCentricCoords(const vector<Vector3>& bc)
    {
        m_VecBC = bc;
    }

    const vector<VertexPosNormTanTex>& GetVertices() const
    {
        return m_VecVertices;
    }
    const vector<DWORD>& GetIndices() const
    {
        return m_VecIndices;
    }
    const vector<DWORD>& GetTetra() const
    {
        return m_VecTetra;
    }
    const vector<Vector3>& GetBaryCentricCoords() const
    {
        return m_VecBC;
    }

    void SetEffect(Effect* effect)
    {
	    m_pEffect = effect;
        CreateInputLayout();
    }   
    Effect* GetEffect() const
    {
	    return m_pEffect;
    }  
    void CreateInputLayout()
    {    
        SafeRelease(m_pInputLayout);

        // Define the input layout
        vector<D3D10_INPUT_ELEMENT_DESC> veclayout;
        UINT numElements;
        GetInputElementDesc<VertexPosNormTanTex>(veclayout, numElements);

        D3D10_PASS_DESC PassDesc;
        m_pEffect->GetCurrentTechnique()->GetPassByIndex(0)->GetDesc(&PassDesc);

        HR(m_pDevice->CreateInputLayout(&veclayout[0], numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pInputLayout));

        m_VertexBufferStride = sizeof(VertexPosNormTanTex);
    }

    void SetName(const tstring& name)
    {
        m_Name = name;
    }

    void Draw()
    {
        ASSERT(m_pVertexBuffer != 0 && m_pIndexBuffer != 0 && m_pEffect != 0);

	    m_pDevice->IASetInputLayout(m_pInputLayout);

        // Set vertex buffer(s)
        UINT offset = 0;
        m_pDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset);
   	
	    // Set index buffer
	    m_pDevice->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // Set primitive topology
        m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        D3D10_TECHNIQUE_DESC techDesc;
        m_pEffect->GetCurrentTechnique()->GetDesc(&techDesc);
        for(UINT p = 0; p < techDesc.Passes; ++p)
        {
            m_pEffect->GetCurrentTechnique()->GetPassByIndex(p)->Apply(0);
		    m_pDevice->DrawIndexed(m_VecIndices.size(), 0, 0); 
        }
    }

private:
    ID3D10Device* m_pDevice;

	ID3D10Buffer* m_pVertexBuffer;
	ID3D10Buffer* m_pIndexBuffer;

    ID3D10InputLayout* m_pInputLayout;
    UINT m_VertexBufferStride;

	vector<VertexPosNormTanTex> m_VecVertices;
	vector<DWORD> m_VecIndices;
	vector<DWORD> m_VecTetra;
	vector<Vector3> m_VecBC;

    tstring m_Name;

	Effect* m_pEffect;
};

