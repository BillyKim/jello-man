#include "BinCooker.h"
#include "CookingStream.h"

BinCooker::BinCooker(Model* pModel, const string output): m_pModel(pModel), m_OutputName(output)
{
}


BinCooker::~BinCooker(void)
{
}

bool BinCooker::Cook()
{
    if (m_pModel->IsSoftBody())
        return CookSB();
    else
        return CookNSB();
}
bool BinCooker::CookNSB()
{ 
    UserStream stream(m_OutputName.c_str(), false);

    //store Meshes
    stream.storeDword(m_pModel->GetDrawMeshes().size());

    vector<ModelMesh<VertexPosNormTex>*>::const_iterator it;
    for (it = m_pModel->GetDrawMeshes().cbegin(); it != m_pModel->GetDrawMeshes().cend(); ++it)
    {
        ModelMesh<VertexPosNormTex>* mesh = *it;

        //store Name
        stream.storeWord(mesh->GetName().size());
        const char* name = mesh->GetName().c_str();
        stream.storeBuffer(name, mesh->GetName().size());

        //store Vertices
        stream.storeDword(mesh->GetNumVertices());
        const vector<VertexPosNormTex>& v = mesh->GetVertices();
        for (unsigned int i = 0; i < mesh->GetNumVertices(); ++i)
        {
            stream.storeVector3(v[i].position);
            stream.storeVector3(v[i].normal);
            stream.storeVector2(v[i].tex);
        }

        //store Indices
        stream.storeDword(mesh->GetNumIndices() / 3);
        const vector<DWORD>& ind = mesh->GetIndices();
        for (DWORD i = 0; i < mesh->GetNumIndices(); ++i)
        {
            stream.storeDword(ind[i]);
        }
    }
    return true;
}
//LAYOUT
//NxU8 : isSoftbody
//DWord : #meshes
    //Word: nameLength
        //buffer: name
    //DWord : #vert
        //vector3: pos, vector3: norm, vector2: tex     : vertex
    //DWord : #triangles
        //DWord, DWord, DWord : triangle
    //...
bool BinCooker::CookSB()
{
    UserStream stream(m_OutputName.c_str(), false);

    ModelMesh<VertexPNTSoftbody>* mesh = m_pModel->GetSoftbodyDrawMesh();

    //store Name
    stream.storeWord(mesh->GetName().size());
    const char* name = mesh->GetName().c_str();
    stream.storeBuffer(name, mesh->GetName().size());

    //store Vertices
    stream.storeDword(mesh->GetNumVertices());
    const vector<VertexPNTSoftbody>& v = mesh->GetVertices();
    for (unsigned int i = 0; i < mesh->GetNumVertices(); ++i)
    {
        stream.storeVector3(v[i].position);
        stream.storeVector3(v[i].normal);
        stream.storeVector2(v[i].tex);
        stream.storeDword(v[i].tetra);
        stream.storeVector3(v[i].barycentricCoords);
    }

    //store Indices
    stream.storeDword(mesh->GetNumIndices() / 3);
    const vector<DWORD>& ind = mesh->GetIndices();
    for (DWORD i = 0; i < mesh->GetNumIndices(); ++i)
    {
        stream.storeDword(ind[i]);
    }
    return true;
}
//LAYOUT
//Word: nameLength
    //buffer: name
//DWord : #vert
    //vector3: pos, vector3: norm, vector2: tex, DWORD tetra, Vector3 barycentricCoords     : vertex
//DWord : #triangles
    //DWord, DWord, DWord : triangle
//...
