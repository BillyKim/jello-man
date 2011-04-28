#include "D3DUtil.h"

#include "Controls.h"
#include "Blox2D_Engine.h"
#include "GameConfig.h"
#include "Level.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Sound.h"
#include "LightController.h"
#include "Matrix.h"
#include <vector>
#include "EditorGUI.h"
#include "PhysX.h"
#include "PostProcessor.h"
#include "EdgeDetectionPostEffect.h"
#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
#include "PreShadowEffect.h"
#include "boost\thread\thread.hpp"

class AudioEngine;

class MainGame
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	MainGame();
	~MainGame();

	// GENERAL
	void Initialize(GameConfig& refGameConfig);
	void LoadResources(ID3D10Device* pDXDevice, PhysX* pPhysXEngine);
	void UpdateScene(const float dTime);
	void DrawScene();
	void CheckControls();
	void OnResize(ID3D10RenderTargetView* pRTView);

	void Release();
	void LoadScreen();

	bool ResourcesLoaded()
	{ return m_bResourcesLoaded; }

private:

	void UpdatePhysics(const float dTime);

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

	EditorGUI* m_pEditorGUI;

	Level* m_pLevel;

	Camera* m_pEditorCamera;
	Camera* m_pTrackingCamera;

    LightController* m_pLightController;

	AudioEngine* m_pAudioEngine;
	Sound* m_pTestSound;

	PhysX* m_pPhysXEngine;

	RenderContext* m_pRenderContext;

	DeferredRenderer* m_pDeferredRenderer;
    ForwardRenderer* m_pForwardRenderer;
    PostProcessor* m_pPostProcessor;
    EdgeDetectionPostEffect* m_pEdgeDetectionEffect;
    PreShadowEffect* m_pPreShadowEffect;

	boost::thread m_PhysicsThread;
	boost::mutex m_PhysicsSceneLock;
};