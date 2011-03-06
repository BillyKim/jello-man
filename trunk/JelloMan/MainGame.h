#include "D3DUtil.h"

class InputState;
class GameConfig;
class Blox2D;

class MainGame
{
public:
	MainGame();
	~MainGame();

	void Initialize(GameConfig& refGameConfig);
	void UpdateScene(const InputState & refInputState);
	void DrawScene(Blox2D& refBlox2D); 

private:
	
	float m_dTtime;
	Blox2D* m_pBlox2D;

};