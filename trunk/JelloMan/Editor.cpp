#include "Editor.h"
#include "LightDesc.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ContentManager.h"
#include "Infobar.h"
#include "Infopane.h"
#include "Toolbar.h"
#include "EditorLoader.h"

// CONSTRUCTOR - DESTRUCTOR
Editor::Editor(PhysX* pPhysXEngine, ID3D10Device* pDXDevice, Level* pLevel)
	:	m_pCameraBitmap(0),
		m_bUsingCamera(false),
		m_pRenderContext(0),
		m_bMoveable(false),
		m_pMoveGizmo(0),
		m_pRotateGizmo(0),
		m_Mode(EDITOR_MODE_EDITOR),
		m_pPhysXEngine(pPhysXEngine),
		m_pDXDevice(pDXDevice),
        m_pObjectSelecter(new ObjectSelecter(pLevel, pPhysXEngine)),
		m_pLevel(pLevel),
		m_bScaleable(false),
		m_pScaleGizmo(0),
		m_pSnapper(0),
		m_pEditorLoader(0)
{

}

Editor::~Editor()
{
	delete m_pMoveGizmo;
	delete m_pRotateGizmo;
	delete m_pObjectSelecter;
	delete m_pScaleGizmo;
	delete m_pSnapper;
	delete m_pInfobar;
	delete m_pToolbar;
	delete m_pInfopane;
	delete m_pEditorLoader;
}

// GENERAL
void Editor::Initialize()
{
	// CAMERA
	m_pCameraBitmap = Content->LoadImage(_T("../Content/Images/Editor/camera.png"));

	// MOVE GIZMO
	m_pMoveGizmo = new MoveGizmo(m_pObjectSelecter);

	// ROTATE GIZMO
	m_pRotateGizmo = new RotateGizmo(m_pObjectSelecter);

	// SCALE GIZMO
	m_pScaleGizmo = new ScaleGizmo(m_pObjectSelecter);

	// SNAPPER
	m_pSnapper = new Snapper(m_pMoveGizmo, m_pRotateGizmo);

	m_pEditorLoader = new EditorLoader(m_pLevel, 0, m_pDXDevice, m_pPhysXEngine);

	m_pInfobar = new Infobar();

	m_pToolbar = new Toolbar(this, m_pInfobar, m_pEditorLoader);
	m_pToolbar->LoadButtons();

	m_pInfopane = new Infopane(m_pInfobar, m_pToolbar, m_pLevel, m_pEditorLoader, m_pSnapper);
	m_pInfopane->LoadButtons();
}

void Editor::Draw()
{
	BX2D->SetAntiAliasing(false);

	switch (m_Mode)
	{
		case EDITOR_MODE_EDITOR:
		{
			DrawSelectedObjects();
			
			m_pInfopane->Draw();

			BX2D->SetAntiAliasing(true);

			// MOVE GIZMO
       
			if (m_bMoveable)
				m_pMoveGizmo->Draw(m_pRenderContext);
			else if (m_bRotateable)
				m_pRotateGizmo->Draw(m_pRenderContext);
			else if (m_bScaleable)
				m_pScaleGizmo->Draw(m_pRenderContext);

			BX2D->SetAntiAliasing(false);

			m_pToolbar->Draw();
		m_pInfobar->Draw();

			break;
		}
		case EDITOR_MODE_GAME:
		{
			m_pToolbar->Draw();
			m_pInfobar->Draw();

			break;
		}
		case EDITOR_MODE_PLAY:
		{
			break;
		}
	}

	// CAMERA
	if (m_bUsingCamera && m_Mode != EDITOR_MODE_PLAY)
		BX2D->DrawImage(m_pCameraBitmap, BX2D->GetWindowSize().width - 70, 90, true, 0.8f);

	BX2D->SetAntiAliasing(true);

	// SCENE INFO

	int objectsSelected(0);

	for_each(m_pLevel->GetLevelObjects().cbegin(), m_pLevel->GetLevelObjects().cend(), [&](ILevelObject* pObj)
    {
		if (pObj->IsSelected())
			++objectsSelected;
    });

	int lightsSelected(0);

	for_each(m_pRenderContext->GetLightController()->GetLights().cbegin(), m_pRenderContext->GetLightController()->GetLights().cend(), [&](Light* pLight)
    {
		if (pLight->IsSelected())
			++lightsSelected;
    });

	m_pInfobar->SetSceneInfo(m_pRenderContext->GetLightController()->GetLights().size(), lightsSelected, m_pLevel->GetLevelObjects().size(), objectsSelected);
}

void Editor::Tick(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	m_pEditorLoader->SetRenderContext(pRenderContext);

	m_pToolbar->Tick();

	m_Mode = static_cast<EDITOR_MODE>(m_pToolbar->GetEditorState());

	switch (m_Mode)
	{
		case EDITOR_MODE_EDITOR:
		{
			if (!m_pInfopane->IsColorPickerActive())
				m_pObjectSelecter->Tick(m_pRenderContext);

			if (m_pToolbar->GetSnappingOptionsState() == Toolbar::SNAPPING_OPTIONS_ACTIVE)
			{
				m_pInfopane->SetState(Infopane::INFO_STATE_SNAPPINGOPTIONS);
			}
			else
			{
				if (m_pObjectSelecter->GetSelectedObjects().size() >= 1)
				{
					Light* pLight = dynamic_cast<Light*>(m_pObjectSelecter->GetSelectedObjects()[0]);

					if (pLight)
						m_pInfopane->SetState(Infopane::INFO_STATE_LIGHTINFO);
					else
						m_pInfopane->SetState(Infopane::INFO_STATE_LEVELOBJECTINFO);
				}
				else
				{
					if (m_pToolbar->GetLoadState() == Toolbar::LOAD_STATE_MODEL)
						m_pInfopane->SetState(Infopane::INFO_STATE_LOADMODEL);
					else if (m_pToolbar->GetLoadState() == Toolbar::LOAD_STATE_LEVEL)
						m_pInfopane->SetState(Infopane::INFO_STATE_LOADLEVEL);
					else
						m_pInfopane->SetState(Infopane::INFO_STATE_NONE);
				}
			}

			m_pInfopane->Tick(m_pRenderContext);

			if (m_pToolbar->GetGizmoState() != Toolbar::GIZMO_MODE_NONE)
			{
				if (m_pToolbar->GetGizmoState() == Toolbar::GIZMO_MODE_MOVE)
				{
					m_bMoveable = true;
					m_bRotateable = false;
					m_bScaleable = false;
					m_pMoveGizmo->Tick();
				}
				if (m_pToolbar->GetGizmoState() == Toolbar::GIZMO_MODE_ROTATE)
				{
					m_bRotateable = true;
					m_bMoveable = false;
					m_bScaleable = false;
					m_pRotateGizmo->Tick();
				}
				if (m_pToolbar->GetGizmoState() == Toolbar::GIZMO_MODE_SCALE)
				{
					m_bScaleable = true;
					m_bMoveable = false;
					m_bRotateable = false;
					m_pScaleGizmo->Tick();
				}	
			}
			else
			{
				m_bMoveable = false;
				m_bRotateable = false;
				m_bScaleable = false;
			}

			break;
		}
		case EDITOR_MODE_GAME:
		{
			m_pToolbar->DeactivateEditorButtons();
			m_pInfopane->Clear();
			m_pObjectSelecter->DeselectAll();

			break;
		}
		case EDITOR_MODE_PLAY:
		{
			m_pToolbar->DeactivateEditorButtons();
			m_pInfopane->Clear();
			m_pObjectSelecter->DeselectAll();

			break;
		}
	}

	if (m_pToolbar->GetGridMode() == Toolbar::GRID_MODE_OFF)
		m_pLevel->ShowGrid(false);
	else
		m_pLevel->ShowGrid(true);

	// CAMERA
	if (CONTROLS->RightMBDown())
		m_bUsingCamera = true;
	else
		m_bUsingCamera = false;
}

void Editor::DrawSelectedObjects()
{
	for_each(m_pLevel->GetLevelObjects().cbegin(), m_pLevel->GetLevelObjects().cend(), [&](ILevelObject* pObj)
	{
		SimpleObject* pSObj = dynamic_cast<SimpleObject*>(pObj);

		if (pSObj && pSObj->IsSelected() && pSObj->IsUsedForInstancing())
		{
			Matrix matProj = m_pRenderContext->GetCamera()->GetProjection();
			Matrix matView = m_pRenderContext->GetCamera()->GetView();
			Matrix matWorld = Matrix::Identity;

			D3D10_VIEWPORT viewPort;
			viewPort.MinDepth = 0.0f;
			viewPort.MaxDepth = 1.0f;
			viewPort.TopLeftX = 0;
			viewPort.TopLeftY = 0;
			viewPort.Height = static_cast<UINT>(BX2D->GetWindowSize().height);
			viewPort.Width = static_cast<UINT>(BX2D->GetWindowSize().width);

			Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();
			Vector3 length = m_pRenderContext->GetCamera()->GetPosition() - pSObj->GetPosition();
			//float l = length.Length() / 100;

			//Vector3 pos2D(Vector3::Project(pSObj->GetPosition(), &viewPort, matProj, matView, matWorld));

			if (vLook.Dot(length) < 0)
			{
				/*BX2D->SetColor(255, 255, 0, 0.5f);
				BX2D->FillEllipse(pos2D.X, pos2D.Y, 4 / l, 4 / l);*/

				NxBounds3 bounds;
				pSObj->GetActor()->getShapes()[0]->getWorldBounds(bounds);

				NxVec3 mid;
				mid.zero();
				bounds.getCenter(mid);
				NxVec3 dim;
				bounds.getDimensions(dim);

				Vector3 cube[8];

				cube[0] = Vector3(mid.x + (dim.x/2), mid.y + (dim.y/2), mid.z + (dim.z/2));
				cube[1] = Vector3(mid.x - (dim.x/2), mid.y + (dim.y/2), mid.z + (dim.z/2));
				cube[2] = Vector3(mid.x + (dim.x/2), mid.y - (dim.y/2), mid.z + (dim.z/2));
				cube[3] = Vector3(mid.x - (dim.x/2), mid.y - (dim.y/2), mid.z + (dim.z/2));
				cube[4] = Vector3(mid.x + (dim.x/2), mid.y + (dim.y/2), mid.z - (dim.z/2));
				cube[5] = Vector3(mid.x - (dim.x/2), mid.y + (dim.y/2), mid.z - (dim.z/2));
				cube[6] = Vector3(mid.x + (dim.x/2), mid.y - (dim.y/2), mid.z - (dim.z/2));
				cube[7] = Vector3(mid.x - (dim.x/2), mid.y - (dim.y/2), mid.z - (dim.z/2));

				Vector3 cube2D[8];

				for (int i = 0; i < 8; ++i)
				{
					cube2D[i] = Vector3::Project(cube[i], &viewPort, matProj, matView, matWorld);
				}

				Point2D pol1[4];
				pol1[0] = Point2F(cube2D[0].X, cube2D[0].Y);
				pol1[1] = Point2F(cube2D[1].X, cube2D[1].Y);
				pol1[2] = Point2F(cube2D[3].X, cube2D[3].Y);
				pol1[3] = Point2F(cube2D[2].X, cube2D[2].Y);

				Point2D pol2[4];
				pol2[0] = Point2F(cube2D[4].X, cube2D[4].Y);
				pol2[1] = Point2F(cube2D[5].X, cube2D[5].Y);
				pol2[2] = Point2F(cube2D[7].X, cube2D[7].Y);
				pol2[3] = Point2F(cube2D[6].X, cube2D[6].Y);

				Point2D pol3[4];
				pol3[0] = Point2F(cube2D[0].X, cube2D[0].Y);
				pol3[1] = Point2F(cube2D[2].X, cube2D[2].Y);
				pol3[2] = Point2F(cube2D[6].X, cube2D[6].Y);
				pol3[3] = Point2F(cube2D[4].X, cube2D[4].Y);

				Point2D pol4[4];
				pol4[0] = Point2F(cube2D[1].X, cube2D[1].Y);
				pol4[1] = Point2F(cube2D[3].X, cube2D[3].Y);
				pol4[2] = Point2F(cube2D[7].X, cube2D[7].Y);
				pol4[3] = Point2F(cube2D[5].X, cube2D[5].Y);

				Point2D pol5[4];
				pol5[0] = Point2F(cube2D[0].X, cube2D[0].Y);
				pol5[1] = Point2F(cube2D[1].X, cube2D[1].Y);
				pol5[2] = Point2F(cube2D[5].X, cube2D[5].Y);
				pol5[3] = Point2F(cube2D[4].X, cube2D[4].Y);

				Point2D pol6[4];
				pol6[0] = Point2F(cube2D[2].X, cube2D[2].Y);
				pol6[1] = Point2F(cube2D[3].X, cube2D[3].Y);
				pol6[2] = Point2F(cube2D[7].X, cube2D[7].Y);
				pol6[3] = Point2F(cube2D[6].X, cube2D[6].Y);

				BX2D->SetColor(255, 255, 0, 0.2f);
				BX2D->FillPolygon(pol1, 4);
				BX2D->FillPolygon(pol2, 4);
				BX2D->FillPolygon(pol3, 4);
				BX2D->FillPolygon(pol4, 4);
				BX2D->FillPolygon(pol5, 4);
				BX2D->FillPolygon(pol6, 4);
			}
		}
	});
}

// GETTERS
void Editor::Clear()
{
    m_pObjectSelecter->DeselectAll();
}

Editor::LIGHT_MODE Editor::GetLightMode() const
{
	return static_cast<LIGHT_MODE>(m_pToolbar->GetLightMode());
}

Editor::POST_EFFECTS Editor::GetPostFXMode() const
{
	return static_cast<POST_EFFECTS>(m_pToolbar->GetPostEffectsState());
}