#pragma once
#include <map>
#include "D3DUtil.h"
#include "ModelMesh.h"
#include <algorithm>

template <typename T>
class Model
{
public:
   //---------Constructor-Destructor-------------------------------->
    Model(ID3D10Device* device): m_pDevice(device)
    {
    }

    ~Model(void)
    {
	    for_each(m_Meshes.cbegin(), m_Meshes.cend(), DeleteModelMesh<T>);
	    m_Meshes.clear();
        
    }
    //<---------------------------------------------------------------

    ModelMesh<T>* AddMesh(const tstring& name)
    {
	    ModelMesh<T>* mesh = new ModelMesh<T>(m_pDevice, name);
	    m_Meshes[name] = mesh;
	    return mesh;
    }

private:
    ID3D10Device* m_pDevice;
    map<tstring, ModelMesh<T>*> m_Meshes;

};

template <typename T>
void DeleteModelMesh(const pair<tstring, ModelMesh<T>*>& p)
{
	delete p.second;
}

