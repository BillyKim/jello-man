#include "Model.h"
#include <algorithm>


//---------Constructor-Destructor-------------------------------->
Model::Model(ID3D10Device* device): m_pDevice(device)
{
}


Model::~Model(void)
{
    for_each(m_Meshes.cbegin(), m_Meshes.cend(), DeleteModelMesh);
    m_Meshes.clear();
}
void DeleteModelMesh(const pair<tstring, ModelMesh*>& p)
{
    delete p.second;
}
//<---------------------------------------------------------------


ModelMesh* Model::AddMesh(const tstring& name)
{
    ModelMesh* mesh = new ModelMesh(m_pDevice, name);
    m_Meshes[name] = mesh;
    return mesh;
}