#pragma once
#include "stdafx.h"
#include <vector>
#include "Model.h"

class ConvexCooker
{
public:
    ConvexCooker(Model* pModel, const string& outputName);
	virtual ~ConvexCooker(void);

	virtual bool Cook();


private:
    string m_OutputFile;
    Model* m_pModel;

	ConvexCooker(const ConvexCooker& t);
	ConvexCooker& operator=(const ConvexCooker& t);
};
