//=============================================================================
// MainGame by Bastian Damman & Sebastiaan Sprengers
//=============================================================================
#include "MainGame.h"
#include "ContentManager.h"
#include "AudioEngine.h"
#include "xact3.h"
#include "LightDesc.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "LightBehaviourBroken.h"
#include "LightBehaviourRotator.h"
#include "PhysXBox.h"
#include "PhysXSphere.h"
#include "SimpleObject.h"

MainGame::MainGame()	:	m_dTtime(0),
							m_pLevel(0),
							m_pEditorCamera(0),
                            m_pLightController(0),
							m_pAudioEngine(0),
							m_pTestSound(0),
							m_bResourcesLoaded(false),
							m_bDebug(false),
							m_pEditorGUI(0),
							m_Orbs(0),
							m_pTrackingCamera(0),
							m_pPhysXEngine(0),
							m_pDeferredRenderer(0),
							m_pForwardRenderer(0),
							m_pPostProcessor(0),
							m_pEdgeDetectionEffect(0),
							m_pRenderContext(0),
                            m_pPreShadowEffect(0),
							m_pDefaultFont(0),
							m_pHappyFaceFont(0),
							m_pLoadingResourcesFont(0),
							m_LoadingText(_T("")),
							m_AlphaHappyFace(0),
							m_pHappyEngineFont(0)
{

}

MainGame::~MainGame()
{
	delete m_pEditorCamera;
    delete m_pLightController;
	delete m_pAudioEngine;
	delete m_pTestSound;
	delete m_pEditorGUI;
	delete m_pTrackingCamera;
	delete m_pDeferredRenderer;
	delete m_pForwardRenderer;
	delete m_pPostProcessor;
	delete m_pLevel;
	delete m_pRenderContext;

    delete Content;

    delete m_pPhysXEngine;
}

void MainGame::Initialize(GameConfig& refGameConfig)
{
	// TEST - gameconfig word nog file!
	refGameConfig.SetTitle(_T("Happy Engine"));
	refGameConfig.SetWindowWidth(1280);
	refGameConfig.SetWindowHeight(720);
	refGameConfig.SetBlox2DAntiAliasing(true);
	refGameConfig.SetKeyboardLayout(GameConfig::KEYBOARD_LAYOUT_AZERTY);
	refGameConfig.UsePhysX(true);
}

void MainGame::LoadResources(ID3D10Device* pDXDevice)
{
	m_pDefaultFont = Content->LoadTextFormat(_T("Arial"), 12, false,false);
	BX2D->SetFont(m_pDefaultFont);

	m_LoadingText = _T("camera");

	// CAMERA
	m_pEditorCamera = new Camera(	static_cast<int>(BX2D->GetWindowSize().width),
									static_cast<int>(BX2D->GetWindowSize().height),
									true	);
    m_pEditorCamera->LookAt(Vector3(-500, 300, -500), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pEditorCamera->SetLens(BX2D->GetWindowSize().width/BX2D->GetWindowSize().height,PiOver4,0.1f,100000.0f);

	m_pTrackingCamera = new Camera(	static_cast<int>(BX2D->GetWindowSize().width),
									static_cast<int>(BX2D->GetWindowSize().height),
									false	);
	m_pTrackingCamera->LookAt(Vector3(-225, 115, -205), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pTrackingCamera->SetLens(BX2D->GetWindowSize().width/BX2D->GetWindowSize().height,PiOver4,10.0f,10000.0f);

	++m_Orbs;
	m_LoadingText = _T("render engines");

	// RENDERERS
	m_pDeferredRenderer = new DeferredRenderer(pDXDevice);
	m_pForwardRenderer = new ForwardRenderer(pDXDevice);

	m_pPostProcessor = new PostProcessor(	pDXDevice,
											static_cast<int>(BX2D->GetWindowSize().width),
											static_cast<int>(BX2D->GetWindowSize().height));
	
	m_pEdgeDetectionEffect = Content->LoadEffect<EdgeDetectionPostEffect>(_T("../Content/Effects/postEdgeDetection.fx"));
    m_pEdgeDetectionEffect->SetTechnique(0);

	m_pPostProcessor->SetEffect(m_pEdgeDetectionEffect);

	m_pDeferredRenderer->Init(	static_cast<int>(BX2D->GetWindowSize().width),
								static_cast<int>(BX2D->GetWindowSize().height));
	m_pDeferredRenderer->SetClearColor(Vector4(0.1f, 0.1f, 0.9f, 1.0f));
	
    m_pPreShadowEffect = Content->LoadEffect<PreShadowEffect>(_T("../Content/Effects/preShadowmapShader.fx"));

	++m_Orbs;
	m_LoadingText = _T("lightcontroller");

    // LIGHTCONTROLLER
    m_pLightController = new LightController();
	m_pRenderContext = new RenderContext(m_pEditorCamera, m_pLightController);

   /* SpotLight* sp1 = new SpotLight();
        sp1->SetPosition(Vector3(0.0f,600.0f,0.0f));
        sp1->SetColor(Color(0.9f, 0.002f, 0.002f, 1.0f));
        sp1->SetMulitplier(2.0f);
        sp1->SetAttenuationStart(1000);
        sp1->SetAttenuationEnd(2000);
        sp1->SetOpeningsAngle(ToRadians(135));
        sp1->Rotate(Vector3::Forward, Pi / 4.0f * 3.0f);
        sp1->SetShadowMap(pDXDevice, ShadowMapType1024x1024);
        sp1->SetBehaviour(new LightBehaviourRotator(Vector3::Up, 2.5f));
        m_pLightController->AddLight(sp1);

	SpotLight* sp2 = new SpotLight();
        sp2->SetPosition(Vector3(800.0f,600.0f,0.0f));
        sp2->SetColor(Color(0.9f, 0.9f, 0.8f, 1.0f));
        sp2->SetMulitplier(2.0f);
        sp2->SetAttenuationStart(1000);
        sp2->SetAttenuationEnd(2000);
        sp2->SetOpeningsAngle(ToRadians(90));
        sp2->Rotate(Vector3::Forward, PiOver2);
        sp2->SetShadowMap(pDXDevice, ShadowMapType1024x1024);
        m_pLightController->AddLight(sp2);

	SpotLight* sp3 = new SpotLight();
        sp3->SetPosition(Vector3(-800.0f,600.0f,0.0f));
        sp3->SetColor(Color(0.9f, 0.9f, 0.8f, 1.0f));
        sp3->SetMulitplier(2.0f);
        sp3->SetAttenuationStart(1000);
        sp3->SetAttenuationEnd(2000);
        sp3->SetOpeningsAngle(ToRadians(90));
        sp3->Rotate(Vector3::Forward, PiOver2);
        sp3->SetShadowMap(pDXDevice, ShadowMapType1024x1024);
        sp3->SetBehaviour(new LightBehaviourBroken(BrokenLightType_LightBulb));
        m_pLightController->AddLight(sp3);

	PointLightDesc pl;
	    pl.position = Vector3(0.0f,500.0f,0.0f);
	    pl.color = Color(0.7f,0.7f,0.8f, 1.0f);
	    pl.attenuationEnd = 5000;
	    pl.multiplier = 0.5f;
	    m_pLightController->AddLight(new PointLight(pl));*/

	Image* test = Content->LoadImage(_T("../Content/Images/Editor/plight.png"));
	Image* test2 = Content->LoadImage(_T("../Content/Images/Editor/slight.png"));

	++m_Orbs;
	m_LoadingText = _T("PhysX");
		
	// PHYSX
	m_pPhysXEngine = new PhysX();
	m_pPhysXEngine->Init();

	++m_Orbs;
	m_LoadingText = _T("level");

	// LEVEL
	m_pLevel = new Level(pDXDevice);
	m_pLevel->Initialize(m_pPhysXEngine, m_pTrackingCamera);

	++m_Orbs;
	m_LoadingText = _T("audio");

	//// AUDIO
	//tstring projectLocation = tstring(_T("./Audio/Win/JelloMan"));
	//m_pAudioEngine = new AudioEngine(projectLocation);
	//m_pAudioEngine->Initialize();

	/*m_pTestSound = new Sound(m_pAudioEngine,_T("BackgroundMusic"),_T("BackGroundMusicVolume"));
	m_pTestSound->PreLoad();
	m_pTestSound->SetLoopCount(1);
	m_pTestSound->SetVolume(90);*/

	++m_Orbs;
	m_LoadingText = _T("editor GUI");

	// GUI
	m_pEditorGUI = new EditorGUI(m_pPhysXEngine, pDXDevice, m_pLevel);
	m_pEditorGUI->Initialize();

	#if defined DEBUG || _DEBUG
	cout << "---------------------------\n";
    cout << ":::Resources Initialized:::\n";
    cout << "---------------------------\n";
    cout << "\n";
    cout << "\n";
	cout << "   GAME EVENTS   \n";
    cout << "-----------------\n";
	#endif

	m_bResourcesLoaded = true;
}

void MainGame::UpdateScene(const float dTime)
{
	if (m_pEditorGUI->GetMode() == EditorGUI::MODE_GAME)
		CheckControls();
  
    if (CONTROLS->IsKeyPressed('K'))
        m_pLevel->Serialize("../test.level");
    else if (CONTROLS->IsKeyPressed('L'))
        m_pLevel->Deserialize("../test.level");

	if (!m_pAudioEngine)
	{
		// AUDIO
		tstring projectLocation = tstring(_T("../Audio/Win/JelloMan"));
		m_pAudioEngine = new AudioEngine(projectLocation);
		m_pAudioEngine->Initialize();
	}

	// dtime
	m_dTtime = dTime;

    m_pLightController->Tick(dTime);

	if (m_pEditorGUI->GetMode() != EditorGUI::MODE_PLAY)
	{
		m_pLevel->TickCharacter(false);
		m_pEditorCamera->Tick(dTime);
	}
	else
		m_pLevel->TickCharacter(true);

	if (m_pEditorGUI->NewModelLoaded())
	{
		ILevelObject* newLvlObj = m_pEditorGUI->GetNewLevelObject();
		newLvlObj->Init(m_pPhysXEngine);

		Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();
		vLook.Normalize();

		newLvlObj->Translate(m_pRenderContext->GetCamera()->GetPosition() + vLook * 100);

		m_pLevel->AddLevelObject(newLvlObj);
	}

	//m_pAudioEngine->DoWork();
	//m_pTestSound->Tick();

	if (m_pEditorGUI->GetMode() != EditorGUI::MODE_EDITOR)
	{
		UpdatePhysics(dTime);

		m_pLevel->EditorMode(false);
	}
	else
		m_pLevel->EditorMode(true);

	if (m_pEditorGUI->GetShowGridButton()->IsActive())
		m_pLevel->ShowGrid(true);
	else
		m_pLevel->ShowGrid(false);

	/*if (CONTROLS->IsKeyPressed(VK_SPACE))
	{
		if (!m_pTestSound->IsPlaying())
		{
			m_pTestSound->Play();
		}
		else
		{
			m_pTestSound->Pause();
		}
	}
	
	if (CONTROLS->IsKeyPressed(VK_ADD))
	{
		//m_pTestSound->SetVolume(m_pTestSound->GetVolume() + 1);

		
	}
	else if (CONTROLS->IsKeyDown(VK_SUBTRACT))
	{
		m_pTestSound->SetVolume(m_pTestSound->GetVolume() - 1);
	}*/

	if (m_pEditorGUI->GetLightButton()->IsActive())
		m_pDeferredRenderer->SetLightMode(LIGHT_MODE_LIT);
	else
		m_pDeferredRenderer->SetLightMode(LIGHT_MODE_UNLIT);

    if (m_pEditorGUI->GetMode() == EditorGUI::MODE_EDITOR) //needed for raycast to work when object is moved
        m_pPhysXEngine->FetchResults();

	m_pEditorGUI->Tick(m_pRenderContext);

    if (m_pEditorGUI->GetMode() == EditorGUI::MODE_EDITOR)
        m_pPhysXEngine->Simulate(0.0f);
}

void MainGame::DrawScene()
{	
	// --------------------------------------
	//			   RENDER SCENE
	// --------------------------------------
    //Render ShadowMaps
    #pragma region
    const vector<Light*>& lights = m_pRenderContext->GetLightController()->GetLights();
    for_each(lights.cbegin(), lights.cend(), [&](Light* l)
    {
        if (l->HasShadowMap() == true)
        {         
            m_pRenderContext->SetCamera(l->GetShadowCamera());

            l->GetShadowMap()->BeginDraw();

            m_pLevel->DrawShadowMap(m_pRenderContext, m_pPreShadowEffect); 

            l->GetShadowMap()->EndDraw();
        }
    });
    #pragma endregion

    if (m_pEditorGUI->GetMode() == EditorGUI::MODE_PLAY)
		m_pRenderContext->SetCamera(m_pTrackingCamera);
	else
		m_pRenderContext->SetCamera(m_pEditorCamera);

	// POST PROCESS
	if (m_pEditorGUI->GetPostFXButton()->IsActive())
		m_pPostProcessor->Begin();

	    // START DEFERRED
	    m_pDeferredRenderer->Begin();

	        // DRAW
	        m_pLevel->DrawDeferred(m_pRenderContext);

	    // END DEFERRED
	    m_pDeferredRenderer->End(m_pRenderContext);

	    // START FORWARD
	    m_pForwardRenderer->Begin(m_pDeferredRenderer);

	        // DRAW
	        m_pLevel->DrawForward(m_pRenderContext);

	    // END FORWARD
	    m_pForwardRenderer->End();

	// POST PROCESS
	if (m_pEditorGUI->GetPostFXButton()->IsActive())
		m_pPostProcessor->End();
		
	// --------------------------------------

	m_pEditorGUI->Draw();

	BX2D->SetColor(255,255,255);
	BX2D->ShowFPS(m_dTtime,true,0.5f);

	BX2D->SetColor(255,255,255);
	BX2D->SetFont(m_pDefaultFont);
}

void MainGame::CheckControls()
{
	bool bWaitForScene = true;
	while (bWaitForScene)
	{
		if (m_pPhysXEngine->GetScene()->isWritable())
			break;
	}

	if (CONTROLS->IsKeyPressed(VK_SPACE))
	{
		int r = rand() % 3;
        if (r == 0)
        {
		    // LOAD NEW LEVELOBJECT - WITH NORMAL MAP
			SimpleObject* pLevelObject = new SimpleObject();

			pLevelObject->SetModelPath(_T("../Content/Models/sphere50.binobj"));
	
            pLevelObject->SetPhysXModel(new PhysXSphere(50.0f, 1000));

			pLevelObject->SetDiffusePath(_T("../Content/Textures/weapon_diffuse.png"));
			//pLevelObject->SetNormalPath(_T("../Content/Textures/weapon_normal.png"));
			pLevelObject->SetSpecPath(_T("../Content/Textures/weapon_spec.png"));
			pLevelObject->SetGlossPath(_T("../Content/Textures/weapon_gloss.png"));

			pLevelObject->SetRigid(true);

			pLevelObject->Init(m_pPhysXEngine);

			pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());

			m_pLevel->AddLevelObject(pLevelObject);

			pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
        }
        else if (r == 1)
        {
            // LOAD NEW LEVELOBJECT - WITH NORMAL MAP
			SimpleObject* pLevelObject = new SimpleObject();

			pLevelObject->SetModelPath(_T("../Content/Models/box50.binobj"));
	
            pLevelObject->SetPhysXModel(new PhysXBox(Vector3(50, 50, 50), 1000));

			pLevelObject->SetDiffusePath(_T("../Content/Textures/weapon_diffuse.png"));
			//pLevelObject->SetNormalPath(_T("../Content/Textures/weapon_normal.png"));
			pLevelObject->SetSpecPath(_T("../Content/Textures/weapon_spec.png"));
			pLevelObject->SetGlossPath(_T("../Content/Textures/weapon_gloss.png"));

			pLevelObject->SetRigid(true);

			pLevelObject->Init(m_pPhysXEngine);

			pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());

			m_pLevel->AddLevelObject(pLevelObject);

			pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
        }
		else
		{
			SimpleObject* pLevelObject = new SimpleObject();

			pLevelObject->SetModelPath(_T("../Content/Models/jman.binobj"));
			pLevelObject->SetPhysXModel(_T("../Content/Models/jman.nxconcave"));

			pLevelObject->SetDiffusePath(_T("../Content/Textures/weapon_diffuse.png"));
			pLevelObject->SetSpecPath(_T("../Content/Textures/weapon_spec.png"));
			pLevelObject->SetGlossPath(_T("../Content/Textures/weapon_gloss.png"));

			pLevelObject->SetRigid(true);

			pLevelObject->Init(m_pPhysXEngine);

			pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());

			m_pLevel->AddLevelObject(pLevelObject);

			pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000000);
		}
	}
    if (CONTROLS->IsKeyPressed(VK_CONTROL))
	{
        TestSoftbody* pSB = new TestSoftbody(m_pRenderContext->GetCamera()->GetPosition());
        pSB->Init(m_pPhysXEngine);
        m_pLevel->AddLevelObject(pSB);
		pSB->AddSpeed(m_pRenderContext->GetCamera()->GetLook() * 2000);
	}
}

void MainGame::OnResize(ID3D10RenderTargetView* pRTView)
{
	if (m_bResourcesLoaded)
	{
		m_pDeferredRenderer->OnResized(	static_cast<int>(BX2D->GetWindowSize().width),
										static_cast<int>(BX2D->GetWindowSize().height));

		m_pTrackingCamera->OnResize(	static_cast<int>(BX2D->GetWindowSize().width),
										static_cast<int>(BX2D->GetWindowSize().height));

		m_pEditorCamera->OnResize(	static_cast<int>(BX2D->GetWindowSize().width),
									static_cast<int>(BX2D->GetWindowSize().height));
	}
}
void MainGame::Release()
{
	if (m_bResourcesLoaded)
	{
		m_pDeferredRenderer->OnResize();
	}
}

void MainGame::LoadScreen()
{
	BX2D->SetColor(ColorF(ColorF::CornflowerBlue));
	BX2D->FillBackGround();

	BX2D->SetColor(ColorF(ColorF::LightGray));
	BX2D->DrawGrid(3,RectF(0,0,BX2D->GetWindowSize().width,
									BX2D->GetWindowSize().height));

	if (!m_pHappyFaceFont)
		m_pHappyFaceFont = Content->LoadTextFormat(_T("Arial"),200,true,false);
	if (!m_pLoadingResourcesFont)
		m_pLoadingResourcesFont = Content->LoadTextFormat(_T("Arial"), 16, true, false);
	if (!m_pHappyEngineFont)
		m_pHappyEngineFont = Content->LoadTextFormat(_T("Arial"), 30, true, false);

	tstringstream stream;
	stream << _T("Loading: ") << m_LoadingText << _T("...");

	BX2D->SetFont(m_pLoadingResourcesFont);
	BX2D->SetColor(43,43,43);
	BX2D->DrawStringCentered(stream.str(), 0, 300);

	BX2D->SetFont(m_pHappyEngineFont);
	BX2D->DrawStringCentered(_T("HAPPY ENGINE"), 0, 150);

	D2D1_MATRIX_3X2_F rot;
	D2D1MakeRotateMatrix(90,Point2F(BX2D->GetWindowSize().width/2,
									BX2D->GetWindowSize().height/2),&rot);
	BX2D->SetTransform(rot);

	if (m_Orbs / 6.0f > m_AlphaHappyFace)
		m_AlphaHappyFace += 0.002f;

	BX2D->SetColor(255,255,255,m_AlphaHappyFace);
	BX2D->SetFont(m_pHappyFaceFont);
	m_pHappyFaceFont->SetHorizontalAlignment(TEXT_ALIGNMENT_CENTER);
	m_pHappyFaceFont->SetVerticalAlignment(PARAGRAPH_ALIGNMENT_MIDDLE);
	BX2D->DrawStringCentered(_T(":D"),-20);

	BX2D->ResetTransform();

	for (int i = 0; i < 6; ++i)
	{
		if (i <= m_Orbs)
			BX2D->SetColor(255,255,255);
		else
			BX2D->SetColor(255,255,255,0.2f);

		BX2D->FillEllipse(Point2F(BX2D->GetWindowSize().width / 2 + ((i - 2) * 40) - 20, (BX2D->GetWindowSize().height / 2) + 200), 15,15);
	}
}

void MainGame::UpdatePhysics(const float dTime)
{
	m_pPhysXEngine->FetchResults();
	m_pLevel->Tick(dTime);
	m_pPhysXEngine->Simulate(1/60.0f);
}