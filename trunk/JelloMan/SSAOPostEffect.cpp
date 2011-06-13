#include "SSAOPostEffect.h"
#include "ContentManager.h"

SSAOPostEffect::SSAOPostEffect(ID3D10Device* pDXDevice, ID3D10Effect* effect): 
				Effect(pDXDevice, effect), 
                m_pColorMap(GetVariableByName("backBuffer")->AsShaderResource()),
                m_pNormalMap(GetVariableByName("normalSpecMap")->AsShaderResource()),
				m_pPositionMap(GetVariableByName("positionGlossMap")->AsShaderResource()),
				m_pRandomNormalMap(GetVariableByName("randomNormalMap")->AsShaderResource()),
                m_pRandomSize(GetVariableByName("randomSize")->AsScalar()),
                m_pSampleRadius(GetVariableByName("sampleRadius")->AsScalar()),
				m_pIntensity(GetVariableByName("intensity")->AsScalar()),
				m_pScale(GetVariableByName("scale")->AsScalar()),
				m_pBias(GetVariableByName("bias")->AsScalar()),
				m_pMapWidth(GetVariableByName("bbWidth")->AsScalar()),
				m_pMapHeight(GetVariableByName("bbHeight")->AsScalar()),
				m_pWV(GetVariableByName("mtxWorldView")->AsMatrix()),
				m_pMinIterations(GetVariableByName("minIterations")->AsScalar()),
				m_pMaxIterations(GetVariableByName("maxIterations")->AsScalar()),
                m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pColorMap->IsValid() != 0, "");
    ASSERT(m_pNormalMap->IsValid() != 0, "");
	ASSERT(m_pPositionMap->IsValid() != 0, "");
	ASSERT(m_pRandomNormalMap->IsValid() != 0, "");
	ASSERT(m_pRandomSize->IsValid() != 0, "");
	ASSERT(m_pSampleRadius->IsValid() != 0, "");
	ASSERT(m_pIntensity->IsValid() != 0, "");
	ASSERT(m_pScale->IsValid() != 0, "");
	ASSERT(m_pBias->IsValid() != 0, "");
	ASSERT(m_pMapWidth->IsValid() != 0, "");
	ASSERT(m_pMapHeight->IsValid() != 0, "");
	ASSERT(m_pMinIterations->IsValid() != 0, "");
	ASSERT(m_pMaxIterations->IsValid() != 0, "");

    CreateInputLayout<VertexPosTex>(pDXDevice, this, &m_pInputLayout, m_VertexStride);

	m_pRandomNormalMap->SetResource(Content->LoadTexture2D(_T("../Content/Textures/Engine/random_normals.png"))->GetColorMap());

	m_pRandomSize->SetFloat(64.0f);
	m_pSampleRadius->SetFloat(0.1f);
	m_pIntensity->SetFloat(14.0f);
	m_pScale->SetFloat(2.0f);
	m_pBias->SetFloat(0.03f);
	m_pMinIterations->SetInt(8);
	m_pMaxIterations->SetInt(64);
}

SSAOPostEffect::~SSAOPostEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void SSAOPostEffect::SetWorldView(const Matrix& wv)
{
	m_pWV->SetMatrix((float*)const_cast<Matrix&>(wv));
}

void SSAOPostEffect::SetBackbufferMap(ID3D10ShaderResourceView* map)
{
	m_pColorMap->SetResource(map);
}
void SSAOPostEffect::SetPositionGlossMap(ID3D10ShaderResourceView* map)
{
	m_pPositionMap->SetResource(map);
}
void SSAOPostEffect::SetNormalMap(ID3D10ShaderResourceView* map)
{
	m_pNormalMap->SetResource(map);
}

void SSAOPostEffect::SetBackbufferSize(int width, int height)
{
    m_pMapWidth->SetInt(width);
    m_pMapHeight->SetInt(height);
}
Effect* SSAOPostEffect::GetEffect()
{
    return this;
}

ID3D10InputLayout* SSAOPostEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT SSAOPostEffect::GetVertexStride() const
{
    return m_VertexStride;
}

void SSAOPostEffect::SetRadius(float radius)
{
	m_pSampleRadius->SetFloat(radius);
}
void SSAOPostEffect::SetIntensity(float intensity)
{
	m_pIntensity->SetFloat(intensity);
}

void SSAOPostEffect::SetScale(float scale)
{
	m_pScale->SetFloat(scale);
}

void SSAOPostEffect::SetBias(float bias)
{
	m_pBias->SetFloat(bias);
}

void SSAOPostEffect::SetMinIterations(int minIterations)
{
	m_pMinIterations->SetInt(minIterations);
}

void SSAOPostEffect::SetMaxIterations(int maxIterations)
{
	m_pMaxIterations->SetInt(maxIterations);
}

float SSAOPostEffect::GetRadius()
{
	float* f = 0;

	m_pSampleRadius->GetFloat(f);

	return *f;
}

float SSAOPostEffect::GetIntensity()
{
	float* f = 0;

	m_pIntensity->GetFloat(f);

	return *f;
}

float SSAOPostEffect::GetScale()
{
	float* f = 0;

	m_pScale->GetFloat(f);

	return *f;
}

float SSAOPostEffect::GetBias()
{
	float* f = 0;

	m_pBias->GetFloat(f);

	return *f;
}

int SSAOPostEffect::GetMinIterations()
{
	int* i = 0;

	m_pMinIterations->GetInt(i);

	return *i;
}

int SSAOPostEffect::GetMaxIterations()
{
	int* i = 0;

	m_pMaxIterations->GetInt(i);

	return *i;
}