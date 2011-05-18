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
        for_each(m_DrawMeshes.cbegin(), m_DrawMeshes.cend(), DeleteModelMesh<VertexPosNormTanTex>);
        for_each(m_PhysXMeshes.cbegin(), m_PhysXMeshes.cend(), DeleteModelMesh<VertexPos>);
    }
    //<---------------------------------------------------------------

    const vector<ModelMesh<VertexPosNormTanTex>*>& GetDrawMeshes() const { return m_DrawMeshes; }
    ModelMesh<VertexPosNormTanTex>* AddDrawMesh(const string& name)
    {
	    ModelMesh<VertexPosNormTanTex>* mesh = new ModelMesh<VertexPosNormTanTex>(name);
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

    ModelMesh<VertexPNTSoftbody>* GetSoftbodyDrawMesh() const { return m_SoftbodyDrawMesh; }
    ModelMesh<VertexPNTSoftbody>* SetSoftbodyDrawMesh()
    {
	    ModelMesh<VertexPNTSoftbody>* mesh = new ModelMesh<VertexPNTSoftbody>("SB");
	    m_SoftbodyDrawMesh = mesh;
	    return mesh;
    }

    ModelMesh<VertexPos>* GetSoftbodyMesh() const { return m_SoftbodyMesh; }
    ModelMesh<VertexPos>* SetSoftbodyMesh()
    {
	    ModelMesh<VertexPos>* mesh = new ModelMesh<VertexPos>("");
	    m_SoftbodyMesh = mesh;
	    return mesh;
    }

    bool IsSoftBody() { return m_IsSoftbody; }
    void SetIsSoftBody(bool value) { m_IsSoftbody = value; }

private:
    bool m_IsSoftbody;

    vector<ModelMesh<VertexPosNormTanTex>*> m_DrawMeshes;
    vector<ModelMesh<VertexPos>*> m_PhysXMeshes;

    ModelMesh<VertexPos>* m_SoftbodyMesh;
    ModelMesh<VertexPNTSoftbody>* m_SoftbodyDrawMesh;
};

