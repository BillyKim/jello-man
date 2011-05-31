#include "FluidEffect.h"

FluidEffect::FluidEffect(ID3D10Device* pDXDevice, ID3D10Effect* effect): 
				Effect(pDXDevice, effect), 
                m_pMtxWVP(GetVariableByName("mtxWVP")->AsMatrix()),
                m_pCamPos(GetVariableByName("camPos")->AsVector()),
                m_pRainbowTex(GetVariableByName("texRainbow")->AsShaderResource()),
                m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pMtxWVP->IsValid() != 0, "");
    ASSERT(m_pCamPos->IsValid() != 0, "");
    ASSERT(m_pRainbowTex->IsValid() != 0, "");

    CreateInputLayout<VertexPos>(pDXDevice, this, &m_pInputLayout, m_VertexStride);
}


FluidEffect::~FluidEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void FluidEffect::SetWorldViewProjection(const Matrix& wvp)
{
	D3DXMATRIX mat = (D3DXMATRIX)wvp;
    m_pMtxWVP->SetMatrix((float*)mat);
}
void FluidEffect::SetCamPos(const Vector3& camPos)
{
    float v[3];
    camPos.ToFloat(v);
    m_pCamPos->SetFloatVector(v);
}
void FluidEffect::SetRainbowTex(const Texture2D* tex)
{
    m_pRainbowTex->SetResource(tex->GetColorMap());
}

ID3D10InputLayout* FluidEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0, "");
    return m_pInputLayout;
}
UINT FluidEffect::GetVertexStride() const
{
    return m_VertexStride;
}