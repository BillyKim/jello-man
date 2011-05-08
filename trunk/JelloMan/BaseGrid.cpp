#include "BaseGrid.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
BaseGrid::BaseGrid(ID3D10Device* device)	:	m_pDevice(device),
												m_pVertexBuffer(0),
												m_pEffect(0),
												m_mtxWorld(Matrix::Identity)

{
}

BaseGrid::~BaseGrid()
{
	SafeRelease(m_pVertexBuffer);
}

// GENERAL
void BaseGrid::Init()
{
	m_pEffect = Content->LoadEffect<PosColEffect>(_T("../Content/Effects/poscol.fx"));

	BuildVertexBuffer();
}

void BaseGrid::Draw(const RenderContext* pRenderContext)
{
	m_pEffect->SetWorld(m_mtxWorld);
	m_pEffect->SetWorldViewProjection(m_mtxWorld * pRenderContext->GetCamera()->GetViewProjection());

	m_pDevice->IASetInputLayout(m_pEffect->GetInputLayout());

	 // Set vertex buffer(s)
    UINT offset = 0;
    UINT vertexStride = m_pEffect->GetVertexStride();
    m_pDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &vertexStride, &offset);

    // Set primitive topology
	m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

    D3D10_TECHNIQUE_DESC techDesc;
    m_pEffect->GetCurrentTechnique()->GetDesc(&techDesc);
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        m_pEffect->GetCurrentTechnique()->GetPassByIndex(p)->Apply(0);
		m_pDevice->Draw(m_VecVertices.size(), 0); 
    }
}

void BaseGrid::BuildVertexBuffer()
{
	for (int i = 0; i < 101; ++i)
	{
		if (i == 50)
		{
			m_VecVertices.push_back(VertexPosCol(-5000.f,0.f,(float)(5000-(i*100)),0.f,.25f,.5f,.4f));
			m_VecVertices.push_back(VertexPosCol(5000.f,0.f,(float)(5000-(i*100)),0.f,.25f,.5f,.4f));

			m_VecVertices.push_back(VertexPosCol((float)(5000-(i*100)),0.f,-5000.f,0.f,.25f,.5f,.4f));
			m_VecVertices.push_back(VertexPosCol((float)(5000-(i*100)),0.f,5000.f,0.f,.25f,.5f,.4f));
		}
		else
		{
			m_VecVertices.push_back(VertexPosCol(-5000.f,0.f,(float)(5000-(i*100)),0.f,.1f,.2f,.4f));
			m_VecVertices.push_back(VertexPosCol(5000.f,0.f,(float)(5000-(i*100)),0.f,.1f,.2f,.4f));

			m_VecVertices.push_back(VertexPosCol((float)(5000-(i*100)),0.f,-5000.f,0.f,.1f,.2f,.4f));
			m_VecVertices.push_back(VertexPosCol((float)(5000-(i*100)),0.f,5000.f,0.f,.1f,.2f,.4f));
		}
	}

	SafeRelease(m_pVertexBuffer);

	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof( VertexPosCol ) * m_VecVertices.size();
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initData;
	initData.pSysMem = &m_VecVertices[0];

	HR(m_pDevice->CreateBuffer( &bd, &initData, &m_pVertexBuffer ));
}