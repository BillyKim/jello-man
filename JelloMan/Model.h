#pragma once
#include <vector>
#include "D3DUtil.h"
#include "ModelMesh.h"
#include <algorithm>

#include "CameraBase.h"

template <typename T>
class Model
{
public:
   //---------Constructor-Destructor-------------------------------->
    Model(ID3D10Device* device): m_pDevice(device)
    {
    }

    virtual ~Model(void)
    {
	    for_each(m_Meshes.cbegin(), m_Meshes.cend(), DeleteModelMesh<T>);
	    m_Meshes.clear();   
    }
    //<---------------------------------------------------------------

    const vector<ModelMesh<T>*>& GetModelMeshes() const { return m_Meshes; }
    ModelMesh<T>* AddMesh(const tstring& name, TOPOLOGY_TYPE topoType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    {
	    ModelMesh<T>* mesh = new ModelMesh<T>(m_pDevice, name, topoType);
	    m_Meshes.push_back(mesh);
	    return mesh;
    }

    void Draw(Effect* e) const
    {
        for_each(m_Meshes.cbegin(), m_Meshes.cend(), [&](ModelMesh<T>* mesh)
		{
            mesh->Draw(e);
		});       
    }

	void DrawInstanced(Effect* effect, const ID3D10Buffer* pMtxWorldbuffer, int count)
	{
		for_each(m_Meshes.cbegin(), m_Meshes.cend(), [&](ModelMesh<T>* mesh)
		{
			mesh->DrawInstanced(effect, pMtxWorldbuffer, count);
		});
	}

    bool IsInView(Graphics::Camera::CameraBase* pCamera, const Matrix& world = Matrix::Identity)
    {
        vector<ModelMesh<T>*>::const_iterator it = m_Meshes.cbegin();
        for(; it != m_Meshes.cend(); ++it)
        {
            BoundingSphere sphere((*it)->GetBoundingSphere());

            sphere.position = Vector3::Transform(sphere.position, world).XYZ();
            sphere.radius *= max(max(world(0, 0), world(1, 1)), world(2, 2)); //mult by max of scale

            if (pCamera->IsInView(sphere) == true)
                return true;
        }
        return false;
    }

private:
    ID3D10Device* m_pDevice;
    vector<ModelMesh<T>*> m_Meshes;

    //Disable default copy constructor and assignment operator
    Model(const Model&);
    Model& operator=(const Model&);
};

template <typename T>
void DeleteModelMesh(ModelMesh<T>* p)
{
	delete p;
}

