#pragma once
#include "D3DUtil.h"
#include <vector>
#include "vertex.h"
#include "Effect.h"

class SoftbodyMesh
{
public:
    SoftbodyMesh(ID3D10Device* device, const tstring& name);
    virtual ~SoftbodyMesh(void);
   
    void SetIndices(const vector<DWORD>& indices);
    void SetVertices(const vector<VertexPosNormTanTex>& vertices);
    void SetTetra(const vector<DWORD>& tetra);
    void SetBaryCentricCoords(const vector<Vector3>& bc);

    const vector<VertexPosNormTanTex>& GetVertices() const;
    const vector<DWORD>& GetIndices() const;
    const vector<DWORD>& GetTetra() const;
    const vector<Vector3>& GetBaryCentricCoords() const;

    void SetName(const tstring& name);

    SoftbodyMesh* Copy() const;

    void Draw(Effect* effect);

private:
    ID3D10Device* m_pDevice;

	ID3D10Buffer* m_pVertexBuffer;
	ID3D10Buffer* m_pIndexBuffer;

	vector<VertexPosNormTanTex> m_VecVertices;
	vector<DWORD> m_VecIndices;
	vector<DWORD> m_VecTetra;
	vector<Vector3> m_VecBC;

    tstring m_Name;

    //Disable default copyconstructor and assignment operator
    SoftbodyMesh(const SoftbodyMesh&);
    SoftbodyMesh& operator=(const SoftbodyMesh&);
};

