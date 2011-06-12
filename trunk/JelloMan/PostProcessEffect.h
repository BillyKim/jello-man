#pragma once
#include "D3DUtil.h"
#include "Effect.h"

class PostProcessEffect
{
public:
	virtual void SetBackbufferMap(ID3D10ShaderResourceView* map) = 0;
	virtual void SetColorGlowMap(ID3D10ShaderResourceView* map) = 0;
	virtual void SetNormalMap(ID3D10ShaderResourceView* map) = 0;
	virtual void SetPositionGlossMap(ID3D10ShaderResourceView* map) = 0;
	virtual void SetDepthMap(ID3D10ShaderResourceView* map) = 0;
    virtual void SetBackbufferSize(int width, int height) = 0;
    virtual Effect* GetEffect() = 0;
private:
};