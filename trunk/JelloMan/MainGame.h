#include "D3DUtil.h"

#include "Controls.h"
#include "Blox2D.h"
#include "GameConfig.h"
#include "Level.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Sound.h"

class AudioEngine;

class MainGame
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	MainGame();
	~MainGame();

	// GENERAL
	void Initialize(GameConfig& refGameConfig);
	void LoadResources(ID3D10Device* pDXDevice);
	void UpdateScene(const float dTime);
	void DrawScene();
	void OnResize(ID3D10RenderTargetView* pRTView);
	void Release();

private:

	// DATAMEMBERS
	float m_dTtime;
	bool m_bTest;
	
	DirLight m_DirLight;

	Level* m_pLevel;
	Camera* m_pCamera;
	AudioEngine* m_pAudioEngine;
	Sound* m_pTestSound;
};