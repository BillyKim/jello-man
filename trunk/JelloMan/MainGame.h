#include "D3DUtil.h"

#include "Controls.h"
#include "KeyboardState.h"
#include "MouseState.h"
#include "Blox2D.h"

class GameConfig;

class MainGame
{
public:
	MainGame();
	~MainGame();

	void Initialize(GameConfig& refGameConfig);
	void UpdateScene(const float dTime);
	void DrawScene(); 

private:

	float m_dTtime;
};