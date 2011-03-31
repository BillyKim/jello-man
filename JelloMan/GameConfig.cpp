//---------------------------
// Includes
//---------------------------
#include "GameConfig.h"

//---------------------------
// Constructor & Destructor
//---------------------------
GameConfig::GameConfig()	:	m_pGameTitle(new tstring(_T(""))),
								m_WindowWidth(640),
								m_Windowheight(480),
								m_b2DAntiAliasing(false),
								m_KeyboardLayout(KEYBOARD_LAYOUT_AZERTY),
								m_bUsePhysX(false)
{
	// nothing to create
}

GameConfig::~GameConfig()
{
	delete m_pGameTitle;
}

void GameConfig::SetTitle(const tstring& titleRef)
{
	delete m_pGameTitle;
	m_pGameTitle = new tstring(titleRef);
}

Size2D GameConfig::GetWindowSize()
{
	Size2D size;
	size.width = static_cast<float>(m_WindowWidth);
	size.height = static_cast<float>(m_Windowheight);

	return size;
}