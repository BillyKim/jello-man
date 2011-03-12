#pragma once
#include "D3DUtil.h"
#include <vector>
#include "vertex.h"
#include "Effect.h"

template<typename T>
class ModelMesh
{
public:
    ModelMesh(ID3D10Device* device, const tstring& name) : 
			      m_pDevice(device)
			    , m_pIndexBuffer(0)
			    , m_pVertexBuffer(0)
			    , m_pEffect(0)
	            , m_pInputLayout(0)
	            , m_VertexBufferStride(0)
                , m_Name(name)
    {
    }
    ~ModelMesh(void)
    {
	    SafeRelease(m_pVertexBuffer);
	    SafeRelease(m_pIndexBuffer);
        SafeRelease(m_pInputLayout);
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
	    initData.pSysMem = m_VecIndices.data();

	    HR(m_pDevice->CreateBuffer(&ibd, &initData, &m_pIndexBuffer));
    }   
    void SetVertices(vector<T> vertices)
    {
	    m_VecVertices = vertices;

	    //if buffer exists => release
	    SafeRelease(m_pVertexBuffer);

	    D3D10_BUFFER_DESC bd;
	    bd.Usage = D3D10_USAGE_IMMUTABLE;
	    bd.ByteWidth = sizeof( VertexPosNormTex ) * m_VecVertices.size();
	    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	    bd.CPUAccessFlags = 0;
	    bd.MiscFlags = 0;

	    D3D10_SUBRESOURCE_DATA initData;
	    initData.pSysMem = m_VecVertices.data();

	    HR(m_pDevice->CreateBuffer( &bd, &initData, &m_pVertexBuffer ));
    }  
    void SetEffect(Effect* effect)
    {
	    m_pEffect = effect;
        CreateInputLayout();
    }   
    void CreateInputLayout()
    {    
        SafeRelease(m_pInputLayout);

        // Define the input layout
        //;
        vector<D3D10_INPUT_ELEMENT_DESC> veclayout;
        UINT numElements;
        GetInputElementDesc<T>(veclayout, numElements);

        D3D10_PASS_DESC PassDesc;
        m_pEffect->GetCurrentTechnique()->GetPassByIndex(0)->GetDesc(&PassDesc);

        HR(m_pDevice->CreateInputLayout(&veclayout[0], numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pInputLayout));

        m_VertexBufferStride = sizeof(T);
    }


    Effect* GetEffect() const
    {
	    return m_pEffect;
    }  
    void Draw()
    {
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

	vector<T> m_VecVertices;
	vector<DWORD> m_VecIndices;

    tstring m_Name;

	Effect* m_pEffect;
};

