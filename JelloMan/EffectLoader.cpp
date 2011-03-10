#include "StdAfx.h"
#include "EffectLoader.h"
#include "d3dUtil.h"



EffectLoader::EffectLoader(void)
{

}

EffectLoader::~EffectLoader(void)
{
	AssetContainer::~AssetContainer();
}

Effect* EffectLoader::Load(ID3D10Device *pDXDevice, const tstring& key)
{
	HRESULT hr;
	if ( IsAssetPresent(key))
	{
	}
	else
	{
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

		// Create the effect
		ID3D10Blob* ErrorBlob = 0;
		ID3D10Effect* pEffect = 0;
		D3DX10CreateEffectFromFile(key.c_str(), NULL, NULL, "fx_4_0",  dwShaderFlags, 0, 
										 pDXDevice, NULL, NULL, &pEffect, &ErrorBlob, &hr);

		if(FAILED(hr)) 
		{
			if(ErrorBlob)
            {
				SIZE_T size = ErrorBlob->GetBufferSize();
				LPVOID p_Data = ErrorBlob->GetBufferPointer();
				cout << ((LPCSTR)p_Data) << "\n";
				MessageBoxA( NULL, (LPCSTR)p_Data, "D3DX10CreateEffectFromFile Error", MB_OK );
				ErrorBlob->Release();
			}
			else
            {
                cout << "Unknown D3DX10CreateEffectFromFile Error \n";
                MessageBox( NULL, _T("Unknown D3DX10CreateEffectFromFile Error"), _T("Error"), MB_OK );
            }
			return 0;
		}
		else
		{
            Effect* e = new Effect(pDXDevice, pEffect);
			AddAsset(key, e);
		}
        
	}
    return GetAsset(key);
}
