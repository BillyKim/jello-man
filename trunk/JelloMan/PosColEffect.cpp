#include "PosColEffect.h"

PosColEffect::PosColEffect(ID3D10Device* pDXDevice, ID3D10Effect* effect): 
				Effect(pDXDevice, effect), 
                m_pMtxWVP(GetVariableBySemantic("WorldViewProjection")->AsMatrix()),
                m_pMtxWorld(GetVariableBySemantic("World")->AsMatrix()),
                m_pInputLayout(0), m_VertexStride(0)
{
    ASSERT(m_pMtxWVP->IsValid());
    ASSERT(m_pMtxWorld->IsValid());

    CreateInputLayout<VertexPosCol>(pDXDevice, this, &m_pInputLayout, m_VertexStride);
}


PosColEffect::~PosColEffect(void)
{
    SafeRelease(m_pInputLayout);
}

void PosColEffect::SetWorldViewProjection(const Matrix& wvp)
{
	D3DXMATRIX mat = (D3DXMATRIX)wvp;
    m_pMtxWVP->SetMatrix((float*)mat);
}
void PosColEffect::SetWorld(const Matrix& world)
{
	D3DXMATRIX mat = (D3DXMATRIX)world;
    m_pMtxWorld->SetMatrix((float*)mat);
}

ID3D10InputLayout* PosColEffect::GetInputLayout() const
{
    ASSERT(m_pInputLayout != 0);
    return m_pInputLayout;
}
UINT PosColEffect::GetVertexStride() const
{
    return m_VertexStride;
}