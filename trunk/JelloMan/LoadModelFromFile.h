#pragma once

#include "LevelObject.h"

class LoadModelFromFile
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	LoadModelFromFile();
	virtual ~LoadModelFromFile();

	// GENERAL
	void LoadNewModel();

	// GETTERS
	LevelObject* GetLevelObject()
	{ m_bLevelObjectExtracted = true;
	  return m_pLevelObject; }

	bool IsLoaded() const
	{ return m_bIsLoaded; }

	bool LevelObjectExtracted() const
	{ return m_bLevelObjectExtracted; }

private:

	// DATAMEMBERS
	bool m_bIsLoaded;
	bool m_bLevelObjectExtracted;

	LevelObject* m_pLevelObject;
};

