#include "Infopane.h"
#include "LoadModelFromFile.h"
#include "LoadLevelFromFile.h"
#include "ContentManager.h"
#include "Level.h"
#include "Infobar.h"
#include "Toolbar.h"
#include "LightInfo.h"
#include "LevelObjectInfo.h"
#include "RenderContext.h"
#include "ColorPicker.h"
#include "EditorLoader.h"
#include "Snapper.h"

/* CONSTRUCTOR - DESTRUCTOR */
Infopane::Infopane(Infobar* pInfobar, Toolbar* pToolbar, Level* pLevel, EditorLoader* pLoader, Snapper* pSnapper) :	m_pInfobar(pInfobar),
																													m_pLevel(pLevel),
																													m_pToolbar(pToolbar),
																													m_State(INFO_STATE_NONE),
																													m_pLoader(pLoader),
																													m_pSnapper(pSnapper),
																													m_bInit(true),
																													m_bSetLightColor(false),
																													m_pALOptions(0)
{
	m_pLightInfo = new LightInfo();
	m_pLevelObjectInfo = new LevelObjectInfo(pLevel);
	m_pColorPicker = new ColorPicker();
	m_pLoadLevelFromFile = new LoadLevelFromFile();
	m_pLoadModelFromFile = new LoadModelFromFile();
	m_pALOptions = new ALOptions();
}

Infopane::~Infopane()
{
	for_each(m_Buttons.begin(), m_Buttons.end(), [&](pair<string, Button*> p)
	{
		delete p.second;
	});

	delete m_pColorPicker;
	delete m_pLightInfo;
	delete m_pLevelObjectInfo;
	delete m_pLoadLevelFromFile;
	delete m_pLoadModelFromFile;
	delete m_pALOptions;
}

/* GENERAL */
void Infopane::LoadButtons()
{
	// COLOR PICKER BUTTON
	m_Buttons["COLORPICKER"] = new Button(100,217,36,36,true);

	m_ButtonImages["COLORPICKER"].push_back(Content->LoadImage(_T("../Content/Images/Editor/colorpicker_normal.png")));
	m_ButtonImages["COLORPICKER"].push_back(Content->LoadImage(_T("../Content/Images/Editor/colorpicker_hover.png")));

	m_Buttons["COLORPICKER"]->SetNormalState(m_ButtonImages["COLORPICKER"][0]);
	m_Buttons["COLORPICKER"]->SetHoverState(m_ButtonImages["COLORPICKER"][1]);
	m_Buttons["COLORPICKER"]->SetDownState(m_ButtonImages["COLORPICKER"][1]);
	m_Buttons["COLORPICKER"]->SetDeactivatedState(m_ButtonImages["COLORPICKER"][0]);
	m_Buttons["COLORPICKER"]->SetDeactivatedStateHover(m_ButtonImages["COLORPICKER"][1]);
	m_Buttons["COLORPICKER"]->SetDeactivatedStateDown(m_ButtonImages["COLORPICKER"][1]);

	m_Buttons["COLORPICKER"]->SetState(Button::STATE_DEACTIVATED);

	// APPLY BUTTON
	m_Buttons["APPLY"] = new Button((int)(210 + 360),(int)(60 + 220),36,36);

	m_ButtonImages["APPLY"].push_back(Content->LoadImage(_T("../Content/Images/Editor/apply_normal.png")));
	m_ButtonImages["APPLY"].push_back(Content->LoadImage(_T("../Content/Images/Editor/apply_hover.png")));

	m_Buttons["APPLY"]->SetNormalState(m_ButtonImages["APPLY"][0]);
	m_Buttons["APPLY"]->SetHoverState(m_ButtonImages["APPLY"][1]);
	m_Buttons["APPLY"]->SetDownState(m_ButtonImages["APPLY"][1]);
}

void Infopane::Tick(const RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	if (m_bInit)
	{
		m_pLoadLevelFromFile->Tick();
		m_pLoadModelFromFile->Tick();
		m_pLightInfo->Tick(pRenderContext);
		m_pLevelObjectInfo->Tick();

		m_bInit = false;
	}

	switch (m_State)
	{
		case INFO_STATE_NONE:
		{
			break;
		}

		case INFO_STATE_LIGHTINFO:
		{
			m_pLightInfo->Tick(pRenderContext);

			m_Buttons["COLORPICKER"]->Tick();

			m_bSetLightColor = false;

			if (m_Buttons["COLORPICKER"]->IsActive())
			{
				m_Buttons["APPLY"]->Tick();

				if (m_Buttons["APPLY"]->Clicked())
					m_bSetLightColor = true;
			}

			break;
		}

		case INFO_STATE_LEVELOBJECTINFO:
		{
			m_pLevelObjectInfo->Tick();

			break;
		}
		case INFO_STATE_LOADMODEL:
		{
			m_pLoadModelFromFile->Tick();

			if (m_pLoadModelFromFile->Load())
			{
				vector<tstring> paths = m_pLoadModelFromFile->GetPaths();

				m_pLoader->AddLevelObject(paths[0],paths[1],paths[2],paths[3],paths[4],paths[5]);
				
				m_pLoadModelFromFile->Clear();
			}

			break;
		}
		case INFO_STATE_LOADLEVEL:
		{
			m_pLoadLevelFromFile->Tick();

			if (m_pLoadLevelFromFile->Load())
			{
				m_pLoader->LoadLevel(m_pLoadLevelFromFile->GetLoadPath());
			}

			if (m_pLoadLevelFromFile->Save())
			{
				m_pLoader->SaveLevel(m_pLoadLevelFromFile->GetSavePath());
			}

			break;
		}
		case INFO_STATE_SNAPPINGOPTIONS:
		{
			m_pSnapper->Tick();

			break;
		}
		case INFO_STATE_ALOPTIONS:
		{
			m_pALOptions->Tick();

			break;
		}
	}
	
	// buttons
	for_each(m_Buttons.begin(), m_Buttons.end(), [&](pair<string, Button*> p)
	{
		p.second->Tick();
	});
}

void Infopane::Draw()
{
	// background
	BX2D->SetColor(70, 70, 70);
	BX2D->FillRect(0, 50, 200, BX2D->GetWindowSize().height - 71);

	switch (m_State)
	{
		case INFO_STATE_NONE:
		{
			Clear();
			break;
		}

		case INFO_STATE_LIGHTINFO:
		{
			m_pLightInfo->Draw();

			m_Buttons["COLORPICKER"]->Show();

			if (m_Buttons["COLORPICKER"]->IsActive())
			{
				if (m_pLightInfo->GetNrLightsSelected() == 1)
				{
					//TODO
					for (unsigned int i = 0; i < m_pRenderContext->GetLightController()->GetLights().size(); ++i)
					{
						if (m_pRenderContext->GetLightController()->GetLights()[i]->IsSelected())
						{
							Color prevColor = m_pRenderContext->GetLightController()->GetLights()[i]->GetColor() * 255.0f;

							m_pColorPicker->SetPreviousColor(prevColor);

							if (m_bSetLightColor)
							{
								m_pRenderContext->GetLightController()->GetLights()[i]->SetColor(m_pColorPicker->GetCurrentColor() / 255.0f);

								m_pColorPicker->PreviousColorSet(false);

								m_pColorPicker->SetPreviousColor(m_pColorPicker->GetCurrentColor());
							}
						}
					}

					m_pColorPicker->Show();

					m_Buttons["APPLY"]->Show();
				}

				if (m_Buttons["COLORPICKER"]->Clicked())
					m_pColorPicker->PreviousColorSet(false);
			}
			else
			{
				m_pColorPicker->PreviousColorSet(false);
			}

			m_pLevelObjectInfo->HideTextBoxes();
			m_pLoadLevelFromFile->HideTextBoxes();
			m_pLoadModelFromFile->HideTextBoxes();
			m_pSnapper->HideTextBoxes();

			break;
		}

		case INFO_STATE_LEVELOBJECTINFO:
		{
			m_pLevelObjectInfo->Draw(m_pRenderContext);

			m_pLightInfo->HideTextBoxes();
			m_pLoadModelFromFile->HideTextBoxes();
			m_pLoadLevelFromFile->HideTextBoxes();
			m_pSnapper->HideTextBoxes();

			break;
		}
		case INFO_STATE_LOADMODEL:
		{
			m_pLoadModelFromFile->Show();

			m_pLightInfo->HideTextBoxes();
			m_pLevelObjectInfo->HideTextBoxes();
			m_pLoadLevelFromFile->HideTextBoxes();
			m_pSnapper->HideTextBoxes();

			break;
		}
		case INFO_STATE_LOADLEVEL:
		{
			m_pLoadLevelFromFile->Show();

			m_pLightInfo->HideTextBoxes();
			m_pLevelObjectInfo->HideTextBoxes();
			m_pLoadModelFromFile->HideTextBoxes();
			m_pSnapper->HideTextBoxes();

			break;
		}
		case INFO_STATE_SNAPPINGOPTIONS:
		{
			m_pSnapper->Draw();

			m_pLightInfo->HideTextBoxes();
			m_pLevelObjectInfo->HideTextBoxes();
			m_pLoadModelFromFile->HideTextBoxes();
			m_pLoadLevelFromFile->HideTextBoxes();

			break;
		}
		case INFO_STATE_ALOPTIONS:
		{
			m_pALOptions->Draw();

			break;
		}
	}
}

void Infopane::Clear()
{
	m_pLightInfo->HideTextBoxes();
	m_pLevelObjectInfo->HideTextBoxes();
	m_pLoadLevelFromFile->HideTextBoxes();
	m_pLoadModelFromFile->HideTextBoxes();
	m_pSnapper->HideTextBoxes();
}
