#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include "AssetContainer.h"
#include "Effect.h"

class EffectLoader : public AssetContainer<Effect>
{
public:
    //------Constructor-Destructor------->
	EffectLoader(void);
	virtual ~EffectLoader(void);
    //<-----------------------------------

    Effect* Load(ID3D10Device *pDXDevice, const tstring& key);

private:
	EffectLoader(const EffectLoader& t);
	EffectLoader& operator=(const EffectLoader& t);

};
