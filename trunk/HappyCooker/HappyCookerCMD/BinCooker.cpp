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
        stream.storeDword(mesh->GetNumTriangles());
        const vector<NxU32>& ind = mesh->GetIndices();
        for (unsigned int i = 0; i < mesh->GetNumTriangles() * 3; ++i)
        {
            stream.storeDword(ind[i]);
        }
    }
    return true;
}
//LAYOUT
//DWord : #meshes
    //Word: nameLength
        //buffer: name
    //DWord : #vert
        //vector3: pos, vector3: norm, vector2: tex     : vertex
    //DWord : #triangles
        //DWord, DWord, DWord : triangle
    //...
