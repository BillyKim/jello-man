#pragma once
#include <map>
#include "D3DUtil.h"
#include "ModelMesh.h"

class Model
{
public:
    Model(ID3D10Device* device);
    virtual ~Model(void);

    ModelMesh* AddMesh(const tstring& name);

    void Draw();

private:
    ID3D10Device* m_pDevice;
    
    map<tstring, ModelMesh*> m_Meshes;

};
void DeleteModelMesh(const pair<tstring, ModelMesh*>& p);


