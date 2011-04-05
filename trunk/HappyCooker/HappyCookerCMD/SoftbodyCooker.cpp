#include "SoftbodyCooker.h"
#include "CookingStream.h"
#include <NxCooking.h>
#include <NxPhysics.h>

#pragma comment(lib, "PhysXLoader.lib")

SoftbodyCooker::SoftbodyCooker(Model* pModel, const string& outputName):
    m_pModel(pModel),
    m_OutputFile(outputName)
{
}

SoftbodyCooker::~SoftbodyCooker(void)
{
}

bool SoftbodyCooker::Cook()
{
	if (m_pModel->GetSoftbodyMesh() == 0)
    {
        cout << "Error: no Softbodymesh found!\n";
        return false;
    }

	NxCookingInterface *pCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	pCooking->NxInitCooking();
    
    UserStream output(m_OutputFile.c_str(), false);

	bool success = true;

    ModelMesh<VertexPos>* mesh = m_pModel->GetSoftbodyMesh();
        
	// Build the triangle mesh.
    NxSoftBodyMeshDesc softDesc;
	softDesc.numTetrahedra				= mesh->GetNumIndices() / 4;
    softDesc.numVertices    			= mesh->GetNumVertices();
	softDesc.tetrahedra         		= static_cast<const void*>(mesh->GetIndices().data());
    softDesc.vertices					= static_cast<const void*>(mesh->GetVertices().data());
    softDesc.tetrahedronStrideBytes     = mesh->GetNumBytesPerIndex() * 4;
    softDesc.vertexStrideBytes          = mesh->GetNumBytesPerVertex();

    softDesc.vertexMasses               = 0;
	softDesc.flags						= 0;

    success = pCooking->NxCookSoftBodyMesh(softDesc, output);

	pCooking->NxCloseCooking();
	fclose(output.fp);

	return success;
}