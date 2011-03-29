#pragma once

#include "D3DUtil.h"
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "TestObject.h"
#include "TestObject2.h"
#include "Blox2D.h"
#include "RenderContext.h"
#include "PostProcessor.h"
#include "EdgeDetectionPostEffect.h"


class Level
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Level(ID3D10Device* pDXDevice);
	virtual ~Level();

	// GENERAL
	void Initialize();
	void Tick(const float dTime);
	void Draw(const RenderContext* pRenderContext);
	void OnResize(ID3D10RenderTargetView* pRTView);
	void Release();
	void SetLightMode(LightMode lMode);

private:

	// DATAMEMBERS
	ID3D10Device* m_pDXDevice;
    DeferredRenderer* m_pDeferredRenderer;
    ForwardRenderer* m_pForwardRenderer;
    PostProcessor* m_pPostProcessor;

    EdgeDetectionPostEffect* m_pEdgeDetectionEffect;

    TestObject* m_pTestObject;
    TestObject2* m_pTestObject2;

	const RenderContext* m_pRenderContext;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

