#pragma once
#include "D3DUtil.h"

class Effect
{
public:
    //---------Constructor-&-Destructor------------------------->
	Effect(ID3D10Device* graphicsDevice, ID3D10Effect* effect);
	virtual ~Effect(void);
    //<----------------------------------------------------------

    void SetTechnique(int index);
    void SetTechnique(const string& name);

    ID3D10EffectTechnique* GetCurrentTechnique() { return m_pCurrentTechnique; }

    ID3D10EffectVariable* GetVariableByName(const string& name) const;
    ID3D10EffectVariable* GetVariableBySemantic(const string& semantic) const;

protected:
	ID3D10Device* m_pDevice;
	ID3D10Effect* m_pEffect;
	ID3D10EffectTechnique* m_pCurrentTechnique;

private:
	Effect(const Effect& effect);
	Effect& operator=(const Effect& effect);
};
