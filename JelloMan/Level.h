#pragma once

#include "D3DUtil.h"
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "TestObject.h"
#include "TestObject2.h"
#include "Blox2D.h"
#include "RenderContext.h"

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

private:

	// DATAMEMBERS
	ID3D10Device* m_pDXDevice;
    DeferredRenderer* m_pDeferredRenderer;
    ForwardRenderer* m_pForwardRenderer;

    TestObject* m_pTestObject;
    TestObject2* m_pTestObject2;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

