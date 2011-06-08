#pragma once
#include "D3DUtil.h"
#include "Model.h"

class Texture2D;

namespace Instancing{

class IInstancible
{		
public:
	virtual ~IInstancible() {}

	virtual bool IsUsedForInstancing() const = 0;
	virtual tstring GetUniqueInstancingID() const = 0;

	virtual Model<VertexPosNormTanTex>* GetModel() const = 0;
	virtual Texture2D* GetDiffuse() const = 0;
	virtual Texture2D* GetSpec() const = 0;
	virtual Texture2D* GetGloss() const = 0;
	virtual Texture2D* GetNormal() const = 0;

	virtual const Matrix* GetMtxWorldPointer() const = 0;
};

}