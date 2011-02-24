#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "d3dUtil.h"
#include <map>

class EffectLoader
{
public:
	//constructor
	EffectLoader(void);
	virtual ~EffectLoader(void);
	//**Shader Map methods

	/**
	* Adds a Effect object with the provided key.
	* @param key the key to add the Effect object with.
	* @param Effect the XMeshShader object to add.
	*/
	void AddEffect(const tstring& key, ID3D10Effect* shader);
	/**
	* Removes a Effect object from the map.
	* @param key the key to remove.
	*/
	void RemoveEffect(const tstring& key);
	/**
	* Removes all Effect object from the map.
	*/
	void RemoveAllEffects();
	/**
	* Checks if an Effect is present.
	* @return true if the Effect is present, false otherwise.
	*/
	bool IsEffectPresent(const tstring& key)const;
	/**
	* Gets the Effect with the provided key.
	* @param key the Effect key.
	* @return the Effect if an Effect is stored in the map with the provided key, 
	* NULL otherwise.
	*/
	ID3D10Effect* GetEffect(ID3D10Device *pD3DDevice,const tstring& key);

private:
	/**
	* Map with the created effects.
	*/
	map<tstring,ID3D10Effect*> m_Effects;

private:
	EffectLoader(const EffectLoader& t);
	EffectLoader& operator=(const EffectLoader& t);

};
