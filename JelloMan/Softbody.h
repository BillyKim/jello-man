#pragma once
#include "D3DUtil.h"
#include "PhysX.h"
#include "Matrix.h"
#include "Model.h"
#include "RenderContext.h"
#include "SoftbodyMesh.h"
#include "ITransformable.h"

class Softbody : public ITransformable
{
public:
    virtual ~Softbody(void);
    
    void AddForce(const Vector3& force);
    void AddForce(const Vector3& force, const Vector3& pos);
    void AddSpeed(const Vector3& force, const Vector3& pos);
    void AddSpeed(const Vector3& speed);

    Vector3 GetSpeed() const;
    
	virtual void Translate(const Vector3& add);
	virtual void SetPosition(const Vector3& pos);
	virtual Vector3 GetPosition() const;

    virtual void Rotate(const Vector3& axis, float angle);

	virtual void Scale(const Vector3& scale);

protected:
    Softbody();
    void InitSoftbody(PhysX* pPhysX, SoftbodyMesh* pSoftbodyMesh, const tstring& path, const Vector3& pos);

    void SetEffectParams();
    void TransformPositions();
	void SetVertices();

    SoftbodyMesh* m_pSoftbodyMesh;
	vector<VertexPosNormTanTex> m_TransformedVertices;

	float GetRadius() const { return m_Radius; }

	NxSoftBody* m_pSoftbody;
	PhysX* m_pPhysX;

private:
    vector<Vector3> m_vecPositions;
    NxU32 m_numPositions;

    vector<NxU32> m_vecIndices;
    NxU32 m_numIndices;

    Vector3 m_CenterPoint;
    Vector3 m_Dimension;
    float m_Radius;

	/* Disabling default copy constructor and assignment operator */
	Softbody(const Softbody&);
	Softbody& operator=(const Softbody&);
};

