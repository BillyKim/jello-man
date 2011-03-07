#include "D3DUtil.h"

#include "KeyboardState.h"
#include "MouseState.h"
class GameConfig;
class Blox2D;

class MainGame
{
public:
	MainGame();
	~MainGame();

	void Initialize(GameConfig& refGameConfig);
	void UpdateScene(const KeyboardState& refKeyboard, const MouseState& refMouse, const float dTime);
	void DrawScene(Blox2D& refBlox2D); 

private:

	float m_dTtime;
	//bool m_bDraw;

};