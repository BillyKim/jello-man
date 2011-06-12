#include "SoftbodyMesh.h"

SoftbodyMesh::SoftbodyMesh(ID3D10Device* device, const tstring& name) : 
			    m_pDevice(device)
			  , m_pIndexBuffer(0)
			  , m_pVertexBuffer(0)
              , m_Name(name)
{
}
SoftbodyMesh::~SoftbodyMesh(void)
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}
   
void SoftbodyMesh::SetIndices(const vector<DWORD>& indices)
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
void SoftbodyMesh::SetVertices(const vector<VertexPosNormTanTex>& vertices)
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
        ASSERT(vertices.size() == m_VecVertices.size(), "");

        void *pVertices;
        HR(m_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &pVertices));

        //To GPU
        memcpy(pVertices, static_cast<const void*>(vertices.data()), sizeof(VertexPosNormTanTex) * vertices.size());

        m_pVertexBuffer->Unmap();

        //To CPU
        memcpy(static_cast<void*>(m_VecVertices.data()), static_cast<const void*>(vertices.data()), sizeof(VertexPosNormTanTex) * vertices.size());

    }
}  
void SoftbodyMesh::SetTetra(const vector<DWORD>& tetra)
{
    m_VecTetra = tetra;
}
void SoftbodyMesh::SetBaryCentricCoords(const vector<Vector3>& bc)
{
    m_VecBC = bc;
}

const vector<VertexPosNormTanTex>& SoftbodyMesh::GetVertices() const
{
    return m_VecVertices;
}
const vector<DWORD>& SoftbodyMesh::GetIndices() const
{
    return m_VecIndices;
}
const vector<DWORD>& SoftbodyMesh::GetTetra() const
{
    return m_VecTetra;
}
const vector<Vector3>& SoftbodyMesh::GetBaryCentricCoords() const
{
    return m_VecBC;
}

void SoftbodyMesh::SetName(const tstring& name)
{
    m_Name = name;
}


SoftbodyMesh* SoftbodyMesh::Copy() const
{
    SoftbodyMesh* pCopy = new SoftbodyMesh(m_pDevice, m_Name);
    pCopy->SetBaryCentricCoords(m_VecBC);
    pCopy->SetIndices(m_VecIndices);
    pCopy->SetVertices(m_VecVertices);
    pCopy->SetTetra(m_VecTetra);

    return pCopy;
}

void SoftbodyMesh::Draw(Effect* effect)
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
    m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D10_TECHNIQUE_DESC techDesc;
    effect->GetCurrentTechnique()->GetDesc(&techDesc);
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        effect->GetCurrentTechnique()->GetPassByIndex(p)->Apply(0);
		m_pDevice->DrawIndexed(m_VecIndices.size(), 0, 0); 
    }
}

