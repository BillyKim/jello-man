#pragma once
#include "D3DUtil.h"
#include "PhysX.h"
#include "Matrix.h"

class Softbody
{
public:
	virtual ~Softbody(void);

	void InitSoftbody(PhysX* pPhysX, const tstring& path);

private:
	NxSoftBody* m_pSoftbody;
	PhysX* m_pPhysX;
	Matrix m_WorldMatrix;

	//disable constructor
	Softbody();
};

