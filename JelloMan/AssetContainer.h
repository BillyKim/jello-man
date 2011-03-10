#pragma once
#include "D3DUtil.h"
#include <map>
using namespace std;

template<typename T>
class AssetContainer
{
protected:
    AssetContainer(void) { /*m_Map.insert( make_pair<tstring, T*>(_T(""), 0));*/ }
    virtual ~AssetContainer(void) { RemoveAllAssets(); }

	bool IsAssetPresent(const tstring &key) const
    {
	    return m_Map.find(key) != m_Map.end();
    }
    void AddAsset(const tstring &key, T* asset)
    {        
        #if defined DEBUG || _DEBUG
	    wcout << "Adding Asset " << key << "\n";
        #endif
	    m_Map[key] = asset;
    }
	void RemoveAsset(const tstring &key)
    {
	    #if defined DEBUG || _DEBUG
	    wcout << "Releasing Asset " << key << "\n";
        #endif

	    delete m_Map[key];
	    m_Map.erase(key);
    }
	void RemoveAllAssets()
    {
	    map<tstring, T*>::iterator it;
	    for(it = m_Map.begin(); it != m_Map.end(); ++it)
	    {
	        #if defined DEBUG || _DEBUG
	        wcout << "Releasing Asset " << (*it).first << "\n";
            #endif
		    delete (*it).second;
	    }
	    m_Map.clear();
    }
    
    T* GetAsset(const tstring& key)
    {
        return m_Map[key];
    }

private:
    map<tstring, T*> m_Map;

};

