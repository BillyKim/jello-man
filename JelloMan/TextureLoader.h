#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "d3dUtil.h"
#include "TextureLoader.h"
#include <map>

class TextureLoader
{
public:

	//constructor
	TextureLoader(void);
	//destructor
	virtual ~TextureLoader(void);


	/**
	* Adds a Texture object with the provided key.
	* @param key the key to add the Texture object with.
	* @param Texture the XMeshShader object to add.
	*/
	void AddTexture(const tstring &key, ID3D10ShaderResourceView* texture);
	/**
	* Removes a Texture object from the map.
	* @param key the key to remove.
	*/
	void RemoveTexture(const tstring &key);
	/**
	* Removes all Texture object from the map.
	*/
	void RemoveAllTextures();
	/**
	* Checks if an Texture is present.
	* @return true if the Texture is present, false otherwise.
	*/
	bool IsTexturePresent(const tstring &key)const;
	/**
	* Gets the Texture with the provided key.
	* @param key the Texture key.
	* @return the Texture if an Texture is stored in the map with the provided key, 
	* NULL otherwise.
	*/
	ID3D10ShaderResourceView* GetTexture(ID3D10Device *pD3DDevice, const tstring& key);

private:
	/**
	* Map with the created textures.
	*/
	map<tstring,ID3D10ShaderResourceView*> m_Textures;

private:
	TextureLoader(TextureLoader& t);
	TextureLoader& operator=(TextureLoader& t);

};
