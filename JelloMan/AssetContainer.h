#pragma once
#include "D3DUtil.h"
#include <map>
using namespace std;

template<typename T>
class AssetContainer
{
public:
    AssetContainer(void) { }
    virtual ~AssetContainer(void) { RemoveAllAssets(); }

	bool IsAssetPresent(const tstring &key) const
    {
	    return m_Map.find(key) != m_Map.end();
    }
    void AddAsset(const tstring &key, T* asset)
    {        
        #if defined DEBUG || _DEBUG
	    wcout << "Adding Asset: " << key << "\n";
        #endif
	    m_Map[key] = asset;
    }
	void RemoveAsset(const tstring &key)
    {
	    #if defined DEBUG || _DEBUG
	    wcout << "Releasing Asset: " << key << "\n";
        #endif

	    delete m_Map[key];
	    m_Map.erase(key);
    }
	void RemoveAllAssets()
    {
	    for_each(m_Map.begin(), m_Map.end(), [&](pair<tstring, T*> obj)
	    {
	        #if defined DEBUG || _DEBUG
	        tcout << "Releasing Asset: " << obj.first << "\n";
            #endif

			SafeDelete(obj.second);
	    });
	    m_Map.clear();
    }
    
    T* GetAsset(const tstring& key)
    {
        return m_Map[key];
    }

private:
    map<tstring, T*> m_Map;

};

