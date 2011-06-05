#include "Fluid.h"
#include "ContentManager.h"

/* CONSTRUCTOR - DESTRUCTOR */
Fluid::Fluid(NxScene* pScene, NxFluidDesc &desc, const Color& particleColor,  float particleSize, ID3D10Device* pDevice) :
	m_NrParticleBuffer(0),
	m_pFluid(0),
	m_ParticleColor(particleColor),
	m_ParticleSize(particleSize),
	m_MaxParticles(0),
	m_pDevice(pDevice),
	m_pVertexBuffer(0),
	m_pEffect(0),
	m_mtxWorld(Matrix::Identity),
    m_pTexRainbow(Content->LoadTexture2D(_T("../Content/Textures/bubbles/tex_rainbow.png")))
{
	m_MaxParticles = desc.maxParticles;
    m_ParticleBuffer.resize(m_MaxParticles);
	desc.userData = 0;//this;

	// setup particle write data.
	NxParticleData particleData;
	particleData.numParticlesPtr = &m_NrParticleBuffer;
	particleData.bufferPos = &m_ParticleBuffer[0].position.X;
	particleData.bufferPosByteStride = sizeof(Particle);
	particleData.bufferVel = &m_ParticleBuffer[0].velocity.X;
	particleData.bufferVelByteStride = sizeof(Particle);
	particleData.bufferDensity = &m_ParticleBuffer[0].density;
	particleData.bufferDensityByteStride = sizeof(Particle);
	particleData.bufferLife = &m_ParticleBuffer[0].lifetime;
	particleData.bufferLifeByteStride = sizeof(Particle);
	particleData.bufferId = &m_ParticleBuffer[0].id;
	particleData.bufferIdByteStride = sizeof(Particle);
	particleData.bufferDensity = &m_ParticleBuffer[0].density;
	particleData.bufferDensityByteStride = sizeof(Particle);
	
	desc.particlesWriteData = particleData;

	ASSERT(pScene != 0, "PhysX scene error when creating particles");
	m_pFluid = pScene->createFluid(desc);
	ASSERT(m_pFluid != 0, "Fluid creation failed");

	// render
	m_pEffect = Content->LoadEffect<FluidEffect>(_T("../Content/Effects/fluidPreEffect.fx"));
    m_pEffect->SetRainbowTex(m_pTexRainbow);

    BuildVertexBuffer();
}

Fluid::~Fluid()
{
	m_pFluid->getScene().releaseFluid(*m_pFluid);

	SafeRelease(m_pVertexBuffer);
	m_pDevice = 0;
}

/* GENERAL */
void Fluid::Draw(const RenderContext* pRenderContext)
{
    UpdateVertexBuffer();

	m_mtxWorld = Matrix::CreateTranslation(Vector3(0,0,0));

	m_pEffect->SetWorldViewProjection(pRenderContext->GetCamera()->GetViewProjection());
    m_pEffect->SetCamPos(pRenderContext->GetCamera()->GetPosition());

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
		m_pDevice->Draw(m_NrParticleBuffer, 0); 
	}
}
void Fluid::UpdateVertexBuffer()
{
    for (UINT i = 0; i < m_NrParticleBuffer; ++i)
    {
        m_VecVertices[i] = m_ParticleBuffer[i].position;
    }

    void* pData;
    m_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &pData);
    memcpy(pData, &m_VecVertices[0], m_NrParticleBuffer * sizeof(Vector3));
    m_pVertexBuffer->Unmap();
}
void Fluid::BuildVertexBuffer()
{
    m_VecVertices.resize(m_MaxParticles);

    UINT count = 0;
    for_each(m_ParticleBuffer.cbegin(), m_ParticleBuffer.cend(), [&](Particle p)
    {
        m_VecVertices[count] = p.position;
        ++count;
    });

	SafeRelease(m_pVertexBuffer);

	D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof( VertexPos ) * m_MaxParticles;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initData;
	initData.pSysMem = &m_VecVertices[0];

	HR(m_pDevice->CreateBuffer( &bd, &initData, &m_pVertexBuffer ));
}
