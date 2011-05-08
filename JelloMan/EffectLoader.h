#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "D3DUtil.h"
#include "AssetContainer.h"
#include "Effect.h"

class EffectLoader
{
public:
    //------Constructor-Destructor------->
	EffectLoader(void);
	virtual ~EffectLoader(void);
    //<-----------------------------------
	
	template <typename T>
    T* Load(ID3D10Device *pDXDevice, const tstring& key)
    {
	    HRESULT hr;
	    if (m_pAssets->IsAssetPresent(key))
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
			    //dwShaderFlags |= D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;//allow dx9 fx file 
			    dwShaderFlags |= D3D10_SHADER_WARNINGS_ARE_ERRORS;
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
				    //SIZE_T size = ErrorBlob->GetBufferSize();
				    LPVOID p_Data = ErrorBlob->GetBufferPointer();
                    PANIC("!D3DX10CreateEffectFromFile Error: \n!" + string((LPCSTR)p_Data) + "\n!" + string(key.begin(), key.end()));
				    ErrorBlob->Release();
			    }
			    else
                {
                    PANIC("Unknown D3DX10CreateEffectFromFile Error: "+ string(key.begin(), key.end()));
                }
			    return 0;
		    }
		    else
		    {
                T* e = new T(pDXDevice, pEffect);
			    m_pAssets->AddAsset(key, e);
		    }    
	    }
        return dynamic_cast<T*>(m_pAssets->GetAsset(key));
    }

private:
	EffectLoader(const EffectLoader& t);
	EffectLoader& operator=(const EffectLoader& t);

    AssetContainer<Effect>* m_pAssets;
};
