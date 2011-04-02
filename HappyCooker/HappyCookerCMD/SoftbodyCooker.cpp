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
	if (m_pModel->GetPhysXMeshes().size() == 0)
        cout << "Warning: no Softbodymesh found!\n";

	NxCookingInterface *pCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	pCooking->NxInitCooking();
    
    UserStream output(m_OutputFile.c_str(), false);

    vector<ModelMesh<VertexPos>*>::const_iterator it;
	bool success = true;
	for (it = m_pModel->GetPhysXMeshes().cbegin(); it != m_pModel->GetPhysXMeshes().cend(); ++it)
	{
        ModelMesh<VertexPos>* mesh = *it;
        
		// Build the triangle mesh.
        NxSoftBodyMeshDesc softDesc;
		softDesc.numTetrahedra				= mesh->GetNumIndices() / 4;
		softDesc.numVertices    			= mesh->GetNumBytesPerVertex();
		softDesc.tetrahedra         		= static_cast<const void*>(mesh->GetIndices().data());
        softDesc.vertices					= static_cast<const void*>(mesh->GetVertices().data());
        softDesc.tetrahedronStrideBytes     = mesh->GetNumBytesPerIndex();
        softDesc.vertexFlagStrideBytes      = mesh->GetNumBytesPerVertex();

        softDesc.vertexMasses               = 0;
		softDesc.flags						= 0;

        bool imSuccess = pCooking->NxCookSoftBodyMesh(softDesc, output);
		success = success && imSuccess;
	}
	pCooking->NxCloseCooking();
	fclose(output.fp);
	return success;
}