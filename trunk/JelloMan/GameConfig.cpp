//---------------------------
// Includes
//---------------------------
#include "GameConfig.h"

//---------------------------
// Constructor & Destructor
//---------------------------
GameConfig::GameConfig()
{
	// nothing to create
}

GameConfig::~GameConfig()
{
	delete m_pGameTitle;
}

void GameConfig::SetTitle(const tstring& titleRef)
{
	//delete m_pGameTitle;
	m_pGameTitle = new tstring(titleRef);
}

Size2D GameConfig::GetWindowSize()
{
	Size2D size;
	size.width = (float)m_WindowWidth;
	size.height = (float)m_Windowheight;

	return size;
}