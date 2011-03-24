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

	void ColorPicker(PointLight* pointLight);
	void MoveGizmo(PointLight* pointLight);

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

	vector<Bitmap*> m_pPointLightBitmaps;

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

