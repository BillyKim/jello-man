#include "Fluid.h"
#include "ContentManager.h"

/* CONSTRUCTOR - DESTRUCTOR */
Fluid::Fluid(NxScene* pScene, NxFluidDesc &desc, const Color& particleColor,  float particleSize, ID3D10Device* pDevice) :
	m_NrParticleBuffer(0),
	m_pParticleBuffer(0),
	m_pFluid(0),
	m_ParticleColor(particleColor),
	m_ParticleSize(particleSize),
	m_MaxParticles(0),
	m_pDevice(pDevice),
	m_pVertexBuffer(0),
	m_pEffect(0),
	m_mtxWorld(Matrix::Identity)
{
	m_MaxParticles = desc.maxParticles;
	m_pParticleBuffer = new Particle[m_MaxParticles];
	desc.userData = this;

	// setup particle write data.
	NxParticleData particleData;
	particleData.numParticlesPtr = &m_NrParticleBuffer;
	particleData.bufferPos = &m_pParticleBuffer[0].position.X;
	particleData.bufferPosByteStride = sizeof(Particle);
	particleData.bufferVel = &m_pParticleBuffer[0].velocity.X;
	particleData.bufferVelByteStride = sizeof(Particle);
	particleData.bufferDensity = &m_pParticleBuffer[0].density;
	particleData.bufferDensityByteStride = sizeof(Particle);
	particleData.bufferLife = &m_pParticleBuffer[0].lifetime;
	particleData.bufferLifeByteStride = sizeof(Particle);
	particleData.bufferId = &m_pParticleBuffer[0].id;
	particleData.bufferIdByteStride = sizeof(Particle);
	particleData.bufferDensity = &m_pParticleBuffer[0].density;
	particleData.bufferDensityByteStride = sizeof(Particle);
	
	desc.particlesWriteData = particleData;

	ASSERT(pScene, "PhysX scene error when creating particles");
	m_pFluid = pScene->createFluid(desc);
	ASSERT(m_pFluid, "Fluid creation failed");

	// render
	m_pEffect = Content->LoadEffect<FluidEffect>(_T("../Content/Effects/fluidPreEffect.fx"));
}

Fluid::~Fluid()
{
	delete[] m_pParticleBuffer;
	m_pParticleBuffer = NULL;
	m_pFluid->getScene().releaseFluid(*m_pFluid);

	SafeRelease(m_pVertexBuffer);
	m_pDevice = 0;
}

/* GENERAL */
void Fluid::Draw(const RenderContext* pRenderContext)
{
	BuildVertexBuffer();

	m_mtxWorld = Matrix::CreateTranslation(Vector3(0,0,0));

	m_pEffect->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());

	m_pDevice->IASetInputLayout(m_pEffect->GetInputLayout());

	// Set vertex buffer(s)
	UINT offset = 0;
	UINT vertexStride = m_pEffect->GetVertexStride();
	m_pDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &vertexStride, &offset);

	// Set primitive topology
	m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3D10_TECHNIQUE_DESC techDesc;
	m_pEffect->GetCurrentTechnique()->GetDesc(&techDesc);
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pEffect->GetCurrentTechnique()->GetPassByIndex(p)->Apply(0);
		m_pDevice->Draw(m_VecVertices.size(), 0); 
	}

	/*D3D10_VIEWPORT viewPort;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.Width = BX2D->GetWindowSize().width;
	viewPort.Height = BX2D->GetWindowSize().height;*/

	//for (int i = 0; i < m_MaxParticles; ++i)
	//{
	//	D3DXVECTOR3 pos3D(m_pParticleBuffer[i].position.X, m_pParticleBuffer[i].position.Y, m_pParticleBuffer[i].position.Z);
	//	/*Vector3 pos3D(mParticleBuffer[i].position.x, mParticleBuffer[i].position.y, mParticleBuffer[i].position.z);*/
	//	/*Vector3 pos2D(
	//		Vector3::Project(pos3D, &viewPort, pRenderContext->GetCamera()->GetProjection(), pRenderContext->GetCamera()->GetView(), m_mtxWorld));*/

	//	D3DXVECTOR3 pos2D;
	//	D3DXMATRIX projection(pRenderContext->GetCamera()->GetProjection());
	//	D3DXMATRIX view(pRenderContext->GetCamera()->GetView());
	//	D3DXMATRIX world;

	//	D3DXVec3Project(&pos2D, &pos3D, &viewPort, &projection, &view, 0);

	//	BX2D->SetColor(255,0,255);
	//	BX2D->FillEllipse(pos2D.x, pos2D.y, 5, 5);
	//}
}

void Fluid::BuildVertexBuffer()
{
	m_VecVertices.clear();

	for (int i = 0; i < m_MaxParticles; ++i)
	{
		m_VecVertices.push_back(VertexPos(	m_pParticleBuffer[i].position.X,
												m_pParticleBuffer[i].position.Y,
												m_pParticleBuffer[i].position.Z));
	}

	SafeRelease(m_pVertexBuffer);

	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof( VertexPos ) * m_VecVertices.size();
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initData;
	initData.pSysMem = &m_VecVertices[0];

	HR(m_pDevice->CreateBuffer( &bd, &initData, &m_pVertexBuffer ));
}
