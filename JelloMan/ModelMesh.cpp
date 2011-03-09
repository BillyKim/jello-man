#include "ModelMesh.h"


ModelMesh::ModelMesh(ID3D10Device* device, const tstring& name) : 
          m_pDevice(0)
        , m_pIndexBuffer(0)
        , m_pVertexBuffer(0)
        , m_pEffect(0)
        , m_pInputLayout(0)
{
}


ModelMesh::~ModelMesh(void)
{
    SafeRelease(m_pVertexBuffer);
    SafeRelease(m_pIndexBuffer);
    SafeRelease(m_pInputLayout);
}


void ModelMesh::SetIndices(vector<DWORD> indices)
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
void ModelMesh::SetVertices(vector<VertexPosNormTex> vertices)
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
void ModelMesh::SetEffect(Effect* effect)
{
    m_pEffect = effect;
    
    SafeRelease(m_pInputLayout);

    // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
        { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 } 
    };
    UINT numElements = sizeof(layout)/sizeof(layout[0]);

    D3D10_PASS_DESC PassDesc;

    m_pEffect->GetCurrentTechnique()->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(m_pDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pInputLayout));

    m_VertexBufferStride = sizeof(VertexPosNormTex);
}