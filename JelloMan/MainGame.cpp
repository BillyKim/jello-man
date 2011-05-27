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
#include "IniReader.h"
#include "FileNotFoundException.h"
#include "IniWriter.h"

using namespace Graphics::Camera;
using namespace IO;

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
							m_pHappyEngineFont(0),
							m_bRunning(true),
							m_bTicked(false)
{

}

MainGame::~MainGame()
{
	m_bRunning = false;
	cout << "waiting for physx thread to end...";
	m_PhysXThread.join(); // wait for thread to stop before closing program

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
    IniReader reader;
    try { reader.Open("config.ini"); }
    catch (exceptions::FileNotFoundException e) { cout << "config.ini not found, loading defaults\n"; }
    
    int width = -1, 
        height = -1;
    tstring keylayout = _T("");
    if (reader.IsOpen())
    {
        try 
        {
            width = reader.GetInt(_T("GFX"), _T("width"));
            height = reader.GetInt(_T("GFX"), _T("height"));
        }
        catch (RootNotFoundException e) { cout << "warning: reading config.ini, root [GFX] not found, width/height set to default\n"; }
        catch (NodeNotFoundException e) {  cout << "warning: reading config.ini, node 'width' or 'height' not found, width/height set to default\n"; }
        catch (ParseFailException e) { cout << "warning: reading config.ini, error while parsing 'width' or 'height', using defaults\n"; }
        try 
        {
            keylayout = reader.GetString(_T("IO"), _T("layout"));
        }
        catch (RootNotFoundException e) { cout << "warning: reading config.ini, root [IO] not found, keylayout set to AZERTY\n"; }
        catch (NodeNotFoundException e) {  cout << "warning: reading config.ini, node 'keylayout' not found, keylayout set to AZERTY\n"; }
    }
    if (width == -1 || height == -1)
    {
        width = 1280;
        height = 720;
    }
    if (keylayout != _T("AZERTY") && keylayout != _T("QWERTY"))
    {
        keylayout = _T("AZERTY");
    }

	refGameConfig.SetTitle(_T("Happy Engine"));
	refGameConfig.SetWindowWidth(width);
	refGameConfig.SetWindowHeight(height);
	refGameConfig.SetBlox2DAntiAliasing(true);

    if (keylayout == _T("AZERTY"))
	    refGameConfig.SetKeyboardLayout(GameConfig::KEYBOARD_LAYOUT_AZERTY);
    else
        refGameConfig.SetKeyboardLayout(GameConfig::KEYBOARD_LAYOUT_QWERTY);

	refGameConfig.UsePhysX(true);

    IniWriter writer;
    writer.Open("config.ini");
    writer.WriteInt(_T("GFX"), _T("width"), width);
    writer.WriteInt(_T("GFX"), _T("height"), height);
    writer.WriteString(_T("IO"), _T("layout"), keylayout);
    writer.Close();
}

void MainGame::LoadResources(ID3D10Device* pDXDevice)
{
	ASSERT(pDXDevice != 0, "DXDevice error while loading resources");
    
	// PHYSX
	m_pPhysXEngine = new PhysX();
	m_pPhysXEngine->Init();

	m_pDefaultFont = Content->LoadTextFormat(_T("Arial"), 12, false,false);
	BX2D->SetFont(m_pDefaultFont);

	m_LoadingText = _T("camera");

	// CAMERA
	m_pEditorCamera = new FlyCamera(static_cast<int>(BX2D->GetWindowSize().width),
									static_cast<int>(BX2D->GetWindowSize().height));
    m_pEditorCamera->LookAt(Vector3(-10, 6, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_pEditorCamera->SetLens(BX2D->GetWindowSize().width/BX2D->GetWindowSize().height,PiOver4,0.1f,1000.0f);

	m_pTrackingCamera = new FollowCamera(static_cast<int>(BX2D->GetWindowSize().width),
        static_cast<int>(BX2D->GetWindowSize().height), m_pPhysXEngine);
	m_pTrackingCamera->SetLens(BX2D->GetWindowSize().width/BX2D->GetWindowSize().height,PiOver4,0.1f,1000.0f);

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
	m_pRenderContext = new RenderContext(m_pEditorCamera, m_pLightController, m_pDeferredRenderer);

	Image* test = Content->LoadImage(_T("../Content/Images/Editor/plight.png"));
	Image* test2 = Content->LoadImage(_T("../Content/Images/Editor/slight.png"));

	++m_Orbs;
	m_LoadingText = _T("PhysX");

	++m_Orbs;
	m_LoadingText = _T("level");

	// LEVEL
	m_pLevel = new Level(pDXDevice);
	m_pLevel->Initialize(m_pPhysXEngine, m_pTrackingCamera);

	m_LoadingText = _T("Instanced PhysX Test");
    for (int x = 0; x < 10; ++x)
            for (int y = 0; y < 10; ++y)
                for (int z = 0; z < 10; ++z)
                {
		            SimpleObject* pLevelObject = new SimpleObject(true);

		            pLevelObject->SetModelPath(_T("../Content/Models/box1.binobj"));
	
                    pLevelObject->SetPhysXModel(new PhysXBox(Vector3(1.0f, 1.0f, 1.0f), 100));

		            pLevelObject->SetDiffusePath(_T("../Content/Textures/test.png"));
		            pLevelObject->SetNormalPath(_T("../Content/Textures/testnormal.png"));
		            //pLevelObject->SetSpecPath(_T("../Content/Textures/weapon_spec.png"));
		            //pLevelObject->SetGlossPath(_T("../Content/Textures/weapon_gloss.png"));

		            pLevelObject->SetRigid(true);

		            pLevelObject->Init(m_pPhysXEngine);

		            //pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());
                    pLevelObject->Translate(Vector3(x*2, y, z*2));

		            m_pLevel->AddLevelObject(pLevelObject);

		            //pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 80000);
                }

	//m_LoadingText = _T("audio");

	// AUDIO
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

	m_PhysXThread = boost::thread(&MainGame::UpdatePhysics, this); // start new thread

	m_bResourcesLoaded = true;
}

void MainGame::UpdateScene(const float dTime)
{
	if (m_pEditorGUI->GetMode() == EditorGUI::MODE_GAME)
		CheckControls();

	//if (!m_pAudioEngine)
	//{
	//	// AUDIO
	//	tstring projectLocation = tstring(_T("../Audio/Win/JelloMan"));
	//	m_pAudioEngine = new AudioEngine(projectLocation);
	//	m_pAudioEngine->Initialize();
	//}

	// dtime
	m_DTimeLock.lock();
	m_dTtime = dTime;
	m_DTimeLock.unlock();

    m_pLightController->Tick(dTime);

	if (m_pEditorGUI->GetMode() != EditorGUI::MODE_PLAY)
	{
		m_pLevel->TickCharacter(false);
		m_pEditorCamera->Tick(dTime);
	}
	else
    {
		m_pLevel->TickCharacter(true);
        m_pTrackingCamera->Tick(dTime);
    }

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
		// PHYSX - THREADING

		
		//m_PhysXThread.join(); // if thread was not finished, wait for it
		//m_PhysXThread = boost::thread(&MainGame::UpdatePhysics, this, dTime); // start new thread

		m_pLevel->Tick(dTime);

		m_TickLock.lock();
		m_bTicked = true;
		m_TickLock.unlock();

		//-----------------------------------

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
        m_pPostProcessor->End(m_pDeferredRenderer);
		
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

	if (CONTROLS->IsKeyDown(VK_SPACE))
	{
		m_pPhysXEngine->GetPhysXLock().lock();

        SimpleObject* pLevelObject = new SimpleObject(true);

		pLevelObject->SetModelPath(_T("../Content/Models/box1.binobj"));
	
        pLevelObject->SetPhysXModel(new PhysXBox(Vector3(1.0f, 1.0f, 1.0f), 100));

		pLevelObject->SetDiffusePath(_T("../Content/Textures/test.png"));
		pLevelObject->SetNormalPath(_T("../Content/Textures/testnormal.png"));
		//pLevelObject->SetSpecPath(_T("../Content/Textures/weapon_spec.png"));
		//pLevelObject->SetGlossPath(_T("../Content/Textures/weapon_gloss.png"));

		pLevelObject->SetRigid(true);

		pLevelObject->Init(m_pPhysXEngine);

		pLevelObject->Translate(m_pRenderContext->GetCamera()->GetPosition());

		m_pLevel->AddLevelObject(pLevelObject);

		pLevelObject->AddForce(m_pRenderContext->GetCamera()->GetLook() * 200000);

		m_pPhysXEngine->GetPhysXLock().unlock();
	}
}

void MainGame::OnResize(ID3D10RenderTargetView* pRTView)
{
	if (m_bResourcesLoaded)
	{
		m_pDeferredRenderer->OnResized(	static_cast<int>(BX2D->GetWindowSize().width),
										static_cast<int>(BX2D->GetWindowSize().height));

		/*m_pTrackingCamera->OnResize(	static_cast<int>(BX2D->GetWindowSize().width),
										static_cast<int>(BX2D->GetWindowSize().height));

		m_pEditorCamera->OnResize(	static_cast<int>(BX2D->GetWindowSize().width),
									static_cast<int>(BX2D->GetWindowSize().height));*/

        m_pPostProcessor->OnResized(	static_cast<int>(BX2D->GetWindowSize().width),
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
		m_AlphaHappyFace += 0.004f;

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

void MainGame::UpdatePhysics()
{
	while (m_bRunning)
	{
		if (m_pEditorGUI->GetMode() != EditorGUI::MODE_EDITOR && m_bTicked)
		{
			m_TickLock.lock(); // reset tick
			{
				m_bTicked = false;
			}
			m_TickLock.unlock();

			float dTime(0);

			m_DTimeLock.lock(); // get dtime from last tick
			{
				dTime = m_dTtime;
			}
			m_DTimeLock.unlock();

			m_pPhysXEngine->GetPhysXLock().lock(); // simulate
			{
				m_pPhysXEngine->Simulate(dTime);
				m_pPhysXEngine->FetchResults();
			}
			m_pPhysXEngine->GetPhysXLock().unlock();
		}
		else
			Sleep(1);
	}
}