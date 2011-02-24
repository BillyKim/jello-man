#include "StdAfx.h"
#include "EffectLoader.h"
#include "d3dUtil.h"



EffectLoader::EffectLoader(void)
{

}

EffectLoader::~EffectLoader(void)
{
	RemoveAllEffects();
}

void EffectLoader::AddEffect(const tstring& key, ID3D10Effect* shader)
{
	#if defined (_DEBUG) || defined (_DEBUG)
		tstringstream strstr;
		strstr<<_T("Adding Effect ")<<key<<_T("\n");
		OutputDebugString(strstr.str().c_str());
	#endif

	m_Effects[key]=shader;
}

void EffectLoader::RemoveEffect(const tstring& key)
{
	#if defined (_DEBUG) || defined (_DEBUG)
		tstringstream strstr;
		strstr<<_T("Removing Effect ")<<key<<_T("\n");
		OutputDebugString(strstr.str().c_str());
	#endif

	SafeRelease(m_Effects[key]);
	m_Effects.erase(key);
}

void EffectLoader::RemoveAllEffects()
{
	#if defined (_DEBUG) || defined (_DEBUG)
		OutputDebugString(_T("Releasing all ID3DEffects.\n"));
	#endif
	map<tstring,ID3D10Effect*>::iterator it;
	for(it=m_Effects.begin(); it != m_Effects.end();++it)
	{
		SafeRelease((*it).second);
	}
	m_Effects.clear();
}

bool EffectLoader::IsEffectPresent(const tstring& key)const
{
	map<tstring,ID3D10Effect*>::iterator it;
	return  m_Effects.find(key) != m_Effects.end();
}

ID3D10Effect* EffectLoader::GetEffect(ID3D10Device *pD3DDevice,const tstring& key)
{
	HRESULT hr;
	if ( IsEffectPresent(key))
	{
		return m_Effects[key];
	}
	else
	{
		// Create the effect
		ID3D10Blob * ErrorBlob;

		DWORD dwShaderFlags = 0;//D3D10_SHADER_ENABLE_STRICTNESS;
		#if defined( DEBUG ) || defined( _DEBUG )
			// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
			// Setting this flag improves the shader debugging experience, but still allows 
			// the shaders to be optimized and to run exactly the way they will run in 
			// the release configuration of this program.
			dwShaderFlags |= D3D10_SHADER_DEBUG;
			dwShaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
			dwShaderFlags |= D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;//allow dx9 fx file 
			//dwShaderFlags |= D3D10_SHADER_WARNINGS_ARE_ERRORS;
		#endif
		ID3D10Effect* pEffect;
		D3DX10CreateEffectFromFile( key.c_str(), NULL, NULL, "fx_4_0",  dwShaderFlags, 0, 
										 pD3DDevice, NULL, NULL, &pEffect, &ErrorBlob,&hr );
		if( FAILED( hr ) ) 
		{
			if(ErrorBlob){
				SIZE_T size = ErrorBlob->GetBufferSize();
				LPVOID p_Data = ErrorBlob->GetBufferPointer();
				OutputDebugStringA((LPCSTR)p_Data);
				MessageBoxA( NULL, (LPCSTR)p_Data, "D3DX10CreateEffectFromFile Error", MB_OK );
				ErrorBlob->Release();
			}
			else MessageBox( NULL, _T("Unknown D3DX10CreateEffectFromFile Error"), _T("Error"), MB_OK );
			return 0;
		}
		else
		{
			//shader goed compiled, voeg toe aan std::map
			AddEffect(key,pEffect);

			if ( IsEffectPresent(key))
			{
				return m_Effects[key];
			}
			else return 0;
		}
	}
}
