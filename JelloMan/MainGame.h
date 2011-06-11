#include "D3DUtil.h"

#include "PhysX.h"
#include "Controls.h"
#include "Blox2D.h"
#include "GameConfig.h"
#include "Level.h"
#include "RenderContext.h"
#include "FlyCamera.h"
#include "FollowCamera.h"
#include "Sound.h"
#include "LightController.h"
#include "Matrix.h"
#include <vector>
#include "Editor.h"
#include "PostProcessor.h"
#include "EdgeDetectionPostEffect.h"
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"

#pragma warning(disable:4005)
#include "boost\thread\thread.hpp"
#pragma warning(default:4005)


class AudioEngine;

class MainGame
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	MainGame();
	virtual ~MainGame();

	// GENERAL
	void Initialize(GameConfig& refGameConfig);
	void LoadResources(ID3D10Device* pDXDevice);
	void UpdateScene(const float dTime);
	void DrawScene();
	void CheckControls();
	void OnResize(ID3D10RenderTargetView* pRTView);

	void Release();
	void LoadScreen();

	bool ResourcesLoaded()
	{ return m_bResourcesLoaded; }

private:

	void UpdatePhysics();

	// DATAMEMBERS
	float m_dTtime;

	bool m_bResourcesLoaded;
	bool m_bDebug;

	int m_Orbs;
	tstring m_LoadingText;
	float m_AlphaHappyFace;

	TextFormat* m_pDefaultFont;
	TextFormat* m_pHappyFaceFont;
	TextFormat* m_pLoadingResourcesFont;
	TextFormat* m_pHappyEngineFont;

	Editor* m_pEditorGUI;

	Level* m_pLevel;

	Graphics::Camera::FlyCamera* m_pEditorCamera;
	Graphics::Camera::FollowCamera* m_pTrackingCamera;

    LightController* m_pLightController;

	AudioEngine* m_pAudioEngine;
	Sound* m_pTestSound;

	PhysX* m_pPhysXEngine;

	RenderContext* m_pRenderContext;

	DeferredRenderer* m_pDeferredRenderer;
    ForwardRenderer* m_pForwardRenderer;
    PostProcessor* m_pPostProcessor;
    EdgeDetectionPostEffect* m_pEdgeDetectionEffect;

	// PHYSX - THREADING
	boost::thread m_PhysXThread;
	boost::mutex m_DTimeLock;

	bool m_bRunning;

	GameTimer m_PhysXTimer;

	float m_PhysXDTime;
	float m_PhysXTimeBase;

    // Disable default copy constructor and assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};