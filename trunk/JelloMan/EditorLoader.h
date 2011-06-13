#pragma once

#include "D3DUtil.h"

class Level;
class RenderContext;
class PhysX;
class SimpleObject;
class FixedJoint;

class EditorLoader
{
public:

	/* CONSTRUCTOR - DESCTRUCTOR*/
	EditorLoader(	Level* pLevel,
					RenderContext* pRenderContext,
					ID3D10Device* pDXDevice,
					PhysX* pPhysXEngine);

	virtual ~EditorLoader();

	/* LOADERS */
	void AddPointLight();
	void AddSpotLight();
	bool AddLevelObject(	const tstring& modelPath,
							const tstring& physxModelPath,
							const tstring& normalPath,
							const tstring& diffusePath,
							const tstring& specPath,
							const tstring& glossPath	);
	void AddTrigger();
	FixedJoint* AddJoint(SimpleObject* pObj1, SimpleObject* pObj2);

	void LoadLevel(const tstring& fileName);
	void SaveLevel(const tstring& fileName);

	/* SETTERS */
	void SetRenderContext(const RenderContext* pRenderContext)
	{ m_pRenderContext = pRenderContext; }

private:

	/* DATAMEMBERS */
	Level* m_pLevel;

	const RenderContext* m_pRenderContext;

	ID3D10Device* m_pDXDevice;

	PhysX* m_pPhysXEngine;

	/* Disable default assigment operator en copy constructor */
	EditorLoader(const EditorLoader&);
	EditorLoader& operator=(const EditorLoader&);
};

