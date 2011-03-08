#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "d3dUtil.h"
#include "AssetContainer.h"
#include "Effect.h"

class EffectLoader : public AssetContainer<Effect>
{
public:
    //------Constructor-Destructor------->
	EffectLoader(void);
	virtual ~EffectLoader(void);
    //<-----------------------------------

    Effect* Load(ID3D10Device *pD3DDevice, const tstring& key);

private:
	EffectLoader(const EffectLoader& t);
	EffectLoader& operator=(const EffectLoader& t);

};
