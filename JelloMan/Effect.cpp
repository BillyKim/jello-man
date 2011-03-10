#include "stdafx.h"
#include "Effect.h"
#include "vertex.h"

Effect::Effect(ID3D10Device* graphicsDevice, ID3D10Effect* effect):
	m_pCurrentTechnique(0),
	m_pDevice(graphicsDevice),
	m_pEffect(effect)
{
	SetTechnique(0);
}


Effect::~Effect(void)
{
    SafeRelease(m_pEffect);
}

void Effect::SetTechnique(int index)
{
    m_pCurrentTechnique = m_pEffect->GetTechniqueByIndex(index);
    ASSERT(m_pCurrentTechnique->IsValid());
}
void Effect::SetTechnique(const string& name)
{
    m_pCurrentTechnique = m_pEffect->GetTechniqueByName(name.c_str());
    ASSERT(m_pCurrentTechnique->IsValid());
}
ID3D10EffectVariable* Effect::GetVariableByName(const string& name) const
{
    return m_pEffect->GetVariableByName(name.c_str());
}
ID3D10EffectVariable* Effect::GetVariableBySemantic(const string& semantic) const
{
    return m_pEffect->GetVariableBySemantic(semantic.c_str());
}
