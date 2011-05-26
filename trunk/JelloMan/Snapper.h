#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include "MoveGizmo.h"
#include "RotateGizmo.h"

#include <vector>

class Snapper
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	Snapper(MoveGizmo* pMoveGizmo, RotateGizmo* pRotateGizmo);
	virtual ~Snapper();

	/* GENERAL */
	void Tick();
	void Draw();
	void HideTextBoxes();

	/* SETTERS */

	//* Set the snap size for the move gizmo - accurate to 0.002 (2 mm) *
	void SetLinearSnapSize(float snapSize)
	{ m_LinearSnapSize = snapSize; }

	//* Set the snap size for the rotate gizmo - accurate to 0.02 degrees *
	void SetAngularSnapSize(float snapSize)
	{ m_AngularSnapSize = snapSize; }

	//* Turn on/off grid snapping *
	void SnapToGrid(bool bSnap)
	{ m_bSnapToGrid = bSnap; }

	/* GETTERS */

private:

	/* DATAMEMBERS */
	
	MoveGizmo* m_pMoveGizmo;
	RotateGizmo* m_pRotateGizmo;

	float m_LinearSnapSize;
	float m_AngularSnapSize;

	bool m_bSnapToGrid;
	bool m_bTextBoxesSet;

	TextBox* m_pLinearSnapTextBox;
	TextBox* m_pAngularSnapTextBox;

	Button* m_pUseLinearSnappingButton;
	Button* m_pUseAngularSnappingButton;
	Button* m_pGridSnappingButton;

	vector<Image*> m_UseSnappingButtonImages;

	TextFormat* m_pFont;
};

