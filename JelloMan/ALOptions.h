#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include <vector>
#include <map>

struct SSAOSettings
{
	float radius;
	float intensity;
	float scale;
	float bias;
	int minIterations;
	int maxIterations;
};

class ALOptions
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	ALOptions();
	virtual ~ALOptions();

	/* GENERAL */
	void Tick();
	void Draw();

	/* GETTERS */
	SSAOSettings GetSSAOSettings() const
	{ return m_SSAOSettings; }

	bool UseAO() const
	{ return m_bUseAO; }

private:

	/* DATAMEMBERS */
	bool m_bUseAO;

	SSAOSettings m_SSAOSettings;

	vector<Image*> m_AddSubtractButtonImages;
	vector<Image*> m_UseAOButtonImages;
	map<string, Button*> m_Buttons;

	TextFormat* m_pFont;
};

