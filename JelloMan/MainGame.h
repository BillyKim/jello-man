#include "D3DUtil.h"

#include "Controls.h"
#include "KeyboardState.h"
#include "MouseState.h"
#include "Blox2D.h"

#include "GameConfig.h"

class MainGame
{
public:
	MainGame();
	~MainGame();

	void Initialize(GameConfig& refGameConfig);
	void LoadResources(ID3D10Device* pD3D10Device);
	void UpdateScene(const float dTime);
	void DrawScene(); 

private:
	float m_dTtime;
};