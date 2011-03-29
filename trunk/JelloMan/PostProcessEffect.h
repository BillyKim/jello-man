#pragma once
#include "D3DUtil.h"
#include "Effect.h"

class PostProcessEffect
{
public:
	virtual void SetColorMap(ID3D10ShaderResourceView* map) = 0;
    virtual void SetColorMapSize(int width, int height) = 0;
    virtual Effect* GetEffect() = 0;
private:
};