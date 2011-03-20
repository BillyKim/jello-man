#pragma once

#include "Blox2D.h"
#include "Controls.h"
#include <vector>
#include "Camera.h"
#include "RenderContext.h"
#include "LightController.h"

class EditorGUI
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	EditorGUI();
	virtual ~EditorGUI();

	// GENERAL
	void Initialize();
	void Draw();
	void Tick(const RenderContext* pRenderContext);

	// GETTERS
	Button* GetLightButton() const
	{ return m_pLightButton; }
	Button* GetMoveButton() const
	{ return m_pMoveButton; }
	Button* GetPointlightButton() const
	{ return m_pPointlightButton; }
	bool EditorMode() const;

private:

	void VisualLightDebugger(const Camera* pCamera);

	// DATAMEMBERS

	// BUTTONS
	Button* m_pLightButton;
	vector<Bitmap*> m_pLightButtonBitmaps;
	Button* m_pMoveButton;
	vector<Bitmap*> m_pMoveButtonBitmaps;
	Button* m_pGameModeButton;
	vector<Bitmap*> m_pGameModeButtonBitmaps;
	Button* m_pEditorModeButton;
	vector<Bitmap*> m_pEditorModeButtonBitmaps;
	Button* m_pPointlightButton;
	vector<Bitmap*> m_pPointlightButtonBitmaps;
	Button* m_pSpotlightButton;
	vector<Bitmap*> m_pSpotlightButtonBitmaps;

	Bitmap* m_pCameraBitmap;

	bool m_bUsingCamera;
	bool m_bGameModeDown;
	bool m_bEditorModeDown;

	int m_GameEditorModeSelect;

	const RenderContext* m_pRenderContext;

	// VISUAL LIGHT DEBUGGER
	bool m_bLockX;
	bool m_bLockY;
	bool m_bLockZ;
	bool m_bMoveable;
	bool m_bClick;

	vector<HitRegion*> m_pHitRectLights;
	vector<bool> m_LightsSelected;

	Vector3 m_MousePos;
	vector<D3DXVECTOR3> m_MPos;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	EditorGUI(const EditorGUI& t);
	EditorGUI& operator=(const EditorGUI& t);
};

