#include "StdAfx.h"
#include "EffectLoader.h"



EffectLoader::EffectLoader(void) : m_pAssets(new AssetContainer<Effect>)
{

}

EffectLoader::~EffectLoader(void)
{
	delete m_pAssets;
}
