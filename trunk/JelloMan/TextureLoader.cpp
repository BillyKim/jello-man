#include "StdAfx.h"
#include "TextureLoader.h" 
#include "d3dUtil.h"


TextureLoader::TextureLoader(void)
{

}

TextureLoader::~TextureLoader(void)
{
	RemoveAllTextures();
}

void TextureLoader::AddTexture(const tstring& key, ID3D10ShaderResourceView* texture)
{
	tstringstream strstr;
	strstr<<_T("Adding Texture ")<<key<<_T("\n");
	OutputDebugString(strstr.str().c_str());
	m_Textures[key]=texture;
}

void TextureLoader::RemoveTexture(const tstring& key)
{
	tstringstream strstr;
	strstr<<_T("Removing Texture ")<<key<<_T("\n");
	OutputDebugString(strstr.str().c_str());
	SafeRelease(m_Textures[key]);
	m_Textures.erase(key);
}

void TextureLoader::RemoveAllTextures()
{
	OutputDebugString(_T("Releasing all ID3DTextures.\n"));
	map<tstring,ID3D10ShaderResourceView*>::iterator it;
	for(it=m_Textures.begin(); it != m_Textures.end();++it)
	{
		SafeRelease((*it).second);
	}
	m_Textures.clear();
}

bool TextureLoader::IsTexturePresent(const tstring& key) const
{
	map<tstring,ID3D10ShaderResourceView*>::iterator it;
	return  m_Textures.find(key) != m_Textures.end();
	return false;
}

ID3D10ShaderResourceView* TextureLoader::GetTexture(ID3D10Device *pD3DDevice, const tstring& Filename) 
{
	if ( IsTexturePresent(Filename))
	{
		OutputDebugString(_T("Using Existing Texture.\n"));
	}
	else
	{
		OutputDebugString(_T("Loading New Texture.\n"));
		ID3D10ShaderResourceView *pTextureRV;
		HRESULT hr = D3DX10CreateShaderResourceViewFromFile( pD3DDevice, Filename.c_str(), NULL, NULL, &pTextureRV, NULL );
		if(hr!=S_OK)
		{
			tstringstream tstrstr;
			tstrstr << _T("Loading texture ") << Filename << _T(" Failed  !!!");
			//MessageBox(0,tstrstr.str().c_str(),_T("ERROR"),0);
			return 0;
		}
		AddTexture(Filename, pTextureRV);
	}
	return m_Textures[Filename];
}
