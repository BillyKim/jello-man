#pragma once

#include "D3DUtil.h"

class Level
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Level(ID3D10Device* pDXDevice);
	virtual ~Level();

	// GENERAL
	void Initialize();
	void Tick(const float dTime);
	void Draw();

private:

	// DATAMEMBERS
	ID3D10Device* m_pDXDevice;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	Level(const Level& t);
	Level& operator=(const Level& t);
};

