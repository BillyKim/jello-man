#pragma once

#include "D3DUtil.h"

class Image;

class Sprite
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */

	//* Makes a sprite from the given spritesheet with the given animation steps. *
	//* note: only spritesheets with sprites on X-axis. *
	Sprite(Image* imgSpriteSheet, int nrAnimSteps);
	virtual ~Sprite();

	/* GENERAL */

	//* Draws the sprite at the given coördinates. *
	void Draw(float x, float y, int animStep) const;

	/* GETTERS */

	//* Returns the total size of the spritesheet. *
	Size2D GetSpriteSheetDimensions() const;
	//* Returns the number of animationsteps. *
	int GetNrOfAnimationSteps() const
	{ return m_AnimSteps; }

private:

	/* DATAMEMBERS */
	Image* m_imgSpriteSheet;

	int m_AnimSteps;
	Size2D m_SpriteSize;

	/* DEFAULT COPY & ASSIGNMENT OPERATOR */
	Sprite(const Sprite& yRef);
	Sprite& operator=(const Sprite& yRef);
};

