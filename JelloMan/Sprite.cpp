#include "Sprite.h"

#include "Blox2D.h"

/* CONSTRUCTOR - DESTRUCTOR */
Sprite::Sprite(Image* imgSpriteSheet, int nrAnimSteps) :	m_imgSpriteSheet(imgSpriteSheet),
															m_AnimSteps(nrAnimSteps),
															m_SpriteSize(SizeF(0,0))
{
	m_SpriteSize.width = m_imgSpriteSheet->GetDimensions().width / m_AnimSteps;
	m_SpriteSize.height = m_imgSpriteSheet->GetDimensions().height;
}

Sprite::~Sprite()
{
}

/* GENERAL */
void Sprite::Draw(float x, float y, int animStep) const
{
	D2D1_RECT_F area;
	area.left = x + (m_SpriteSize.width * animStep);
	area.top = y;
	area.right = x + m_SpriteSize.width + (m_SpriteSize.width * animStep);
	area.bottom = y + m_SpriteSize.height;

	BX2D->DrawImage(m_imgSpriteSheet, x, y, area, false);
}

/* GETTERS */
Size2D Sprite::GetSpriteSheetDimensions() const
{
	return m_imgSpriteSheet->GetDimensions();
}