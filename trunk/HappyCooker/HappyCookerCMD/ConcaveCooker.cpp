#include "ConcaveCooker.h"
#include "CookingStream.h"
#include <NxCooking.h>
#include <NxPhysics.h>

#pragma comment(lib, "PhysXLoader.lib")

ConcaveCooker::ConcaveCooker(Model* pModel, const string& outputName):
    m_pModel(pModel),
    m_OutputFile(outputName)
{
}

ConcaveCooker::~ConcaveCooker(void)
{
}

bool ConcaveCooker::Cook()
{
	if (m_pModel->GetPhysXMeshes().size() == 0)
        cout << "Warning: no physXmesh found!\n";

	NxCookingInterface *pCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	pCooking->NxInitCooking();
    
    UserStream output(m_OutputFile.c_str(), false);

    vector<ModelMesh<VertexPos>*>::const_iterator it;
	bool success = true;
	for (it = m_pModel->GetPhysXMeshes().cbegin(); it != m_pModel->GetPhysXMeshes().cend(); ++it)
	{
        ModelMesh<VertexPos>* mesh = *it;
        
		// Build the triangle mesh.
		NxTriangleMeshDesc meshDesc;
		meshDesc.numVertices				= mesh->GetNumVertices();
		meshDesc.numTriangles				= mesh->GetNumIndices() / 3;
		meshDesc.pointStrideBytes			= mesh->GetNumBytesPerVertex();
		meshDesc.triangleStrideBytes		= mesh->GetNumBytesPerIndex() * 3;
        meshDesc.points						= static_cast<const void*>(mesh->GetVertices().data());
		meshDesc.triangles					= static_cast<const void*>(mesh->GetIndices().data());
		meshDesc.flags						= /*NX_MF_FLIPNORMALS|*/0;

		bool imSuccess = pCooking->NxCookTriangleMesh(meshDesc, output);
		success = success && imSuccess;
	}
	pCooking->NxCloseCooking();
	fclose(output.fp);
	return success;
}