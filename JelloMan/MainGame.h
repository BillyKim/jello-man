#include "D3DUtil.h"

#include "Controls.h"
#include "KeyboardState.h"
#include "MouseState.h"
#include "Blox2D.h"
#include "GameConfig.h"
#include "Level.h"
#include "RenderContext.h"
#include "Camera.h"

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

private:

	// DATAMEMBERS
	float m_dTtime;
	
	Level* m_pLevel;

	DirLight m_DirLight;

	Camera* m_pCamera;
};