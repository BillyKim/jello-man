#pragma once
#include <vector>
#include "ModelMesh.h"
#include <algorithm>


template <typename T>
void DeleteModelMesh(ModelMesh<T>* p)
{
	delete p;
}

class Model
{
public:
   //---------Constructor-Destructor-------------------------------->
    Model()
    {
    }

    ~Model(void)
    {
        for_each(m_DrawMeshes.cbegin(), m_DrawMeshes.cend(), DeleteModelMesh<VertexPosNormTex>);
        for_each(m_PhysXMeshes.cbegin(), m_PhysXMeshes.cend(), DeleteModelMesh<VertexPos>);
    }
    //<---------------------------------------------------------------

    const vector<ModelMesh<VertexPosNormTex>*>& GetDrawMeshes() const { return m_DrawMeshes; }
    ModelMesh<VertexPosNormTex>* AddDrawMesh(const string& name)
    {
	    ModelMesh<VertexPosNormTex>* mesh = new ModelMesh<VertexPosNormTex>(name);
	    m_DrawMeshes.push_back(mesh);
	    return mesh;
    }

    const vector<ModelMesh<VertexPos>*>& GetPhysXMeshes() const { return m_PhysXMeshes; }
    ModelMesh<VertexPos>* AddPhysXMesh(const string& name)
    {
	    ModelMesh<VertexPos>* mesh = new ModelMesh<VertexPos>(name);
	    m_PhysXMeshes.push_back(mesh);
	    return mesh;
    }

private:
    vector<ModelMesh<VertexPosNormTex>*> m_DrawMeshes;
    vector<ModelMesh<VertexPos>*> m_PhysXMeshes;

};

