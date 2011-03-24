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

	void VisualPointLightDebugger();
	void VisualSpotLightDebugger();

	void ColorPicker(Color& color);

	void MoveGizmo(Vector3& position, int id);

	void RotateGizmo(Vector3& position, int id);

	// GETTERS
	Button* GetLightButton() const
	{ return m_pLightButton; }
	Button* GetMoveButton() const
	{ return m_pMoveButton; }
	Button* GetPointlightButton() const
	{ return m_pPointlightButton; }
	bool EditorMode() const;

private:

	Vector3 GetHue(int i);
	Vector3 HsvToRgb(double h, double S, double V);
	void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );

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
	Button* m_pColorPickerButton;
	vector<Bitmap*> m_pColorPickerButtonBitmaps;
	Button* m_pApplyButton;
	vector<Bitmap*> m_pApplyButtonBitmaps;
	Button* m_pRotateButton;
	vector<Bitmap*> m_pRotateButtonBitmaps;

	vector<Bitmap*> m_pPointLightBitmaps;
	vector<Bitmap*> m_pSpotLightBitmaps;

	Bitmap* m_pCameraBitmap;

	bool m_bUsingCamera;
	bool m_bGameModeDown;
	bool m_bEditorModeDown;
	bool m_bPreviousColorSet;

	int m_GameEditorModeSelect;

	const RenderContext* m_pRenderContext;

	Vector3 m_PreviousColor;
	Vector3 m_CurrentColor;
	Vector3 m_HueColor;
	Point2D m_ColorPickerPos;
	Point2D m_ColorPickerSelectPos;
	int m_Hue;

	// VISUAL POINTLIGHT DEBUGGER
	vector<HitRegion*> m_pPLightHitRects;
	vector<bool> m_PLightsSelected;

	vector<D3DXVECTOR3> m_OldPointLightPos;

	// VISUAL SPOTLIGHT DEBUGGER
	vector<HitRegion*> m_pSLightHitRects;
	vector<bool> m_SLightsSelected;

	vector<D3DXVECTOR3> m_OldSpotLightPos;

	// VISUAL LIGHT DEBUGGER
	bool m_bLockX;
	bool m_bLockY;
	bool m_bLockZ;
	bool m_bMoveable;
	bool m_bRotateable;
	bool m_bClick;
	bool m_bClick2;

	int m_SelectedLights;
	int m_TotalLights;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	EditorGUI(const EditorGUI& t);
	EditorGUI& operator=(const EditorGUI& t);
};

