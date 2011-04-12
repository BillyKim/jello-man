#pragma once
#include <vector>
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

    const vector<ModelMesh<T>*>& GetModelMeshes() const { return m_Meshes; }
    ModelMesh<T>* AddMesh(const tstring& name)
    {
	    ModelMesh<T>* mesh = new ModelMesh<T>(m_pDevice, name);
	    m_Meshes.push_back(mesh);
	    return mesh;
    }

    void Draw() const
    {
	    for_each(m_Meshes.cbegin(), m_Meshes.cend(), DrawModelMesh<T>);
    }
    void DrawEffectless() const
    {
	    for_each(m_Meshes.cbegin(), m_Meshes.cend(), DrawEffectlessModelMesh<T>);
    }

private:
    ID3D10Device* m_pDevice;
    vector<ModelMesh<T>*> m_Meshes;
};

template <typename T>
void DeleteModelMesh(ModelMesh<T>* p)
{
	delete p;
}
template <typename T>
void DrawModelMesh(ModelMesh<T>* p)
{
	p->Draw();
}
template <typename T>
void DrawEffectlessModelMesh(ModelMesh<T>* p)
{
	p->DrawEffectless();
}

