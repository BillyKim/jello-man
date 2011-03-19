#pragma once

#include "Blox2D.h"
#include "Controls.h"
#include <vector>

class EditorGUI
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	EditorGUI();
	virtual ~EditorGUI();

	// GENERAL
	void Initialize();
	void Draw();
	void Tick();

	// GETTERS
	Button* GetLightButton() const
	{ return m_pLightButton; }
	Button* GetMoveButton() const
	{ return m_pMoveButton; }

private:

	// DATAMEMBERS
	Button* m_pLightButton;
	vector<Bitmap*> m_pLightButtonBitmaps;

	Button* m_pMoveButton;
	vector<Bitmap*> m_pMoveButtonBitmaps;

	Bitmap* m_pCameraBitmap;

	bool m_bUsingCamera;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	EditorGUI(const EditorGUI& t);
	EditorGUI& operator=(const EditorGUI& t);
};

