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
	void OnResize(ID3D10RenderTargetView* pRTView);
	void Release();
	void LoadScreen();

private:

	// DATAMEMBERS
	float m_dTtime;

	bool m_bResourcesLoaded;
	bool m_bDebug;

	int m_Angle;

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
};