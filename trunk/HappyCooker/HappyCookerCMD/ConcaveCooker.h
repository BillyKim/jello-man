#pragma once
#include "stdafx.h"
#include <vector>
#include "Model.h"

class ConcaveCooker
{
public:
    ConcaveCooker(Model* pModel, const string& outputName);
	virtual ~ConcaveCooker(void);

	virtual bool Cook();


private:
    string m_OutputFile;
	Model* m_pModel;

	ConcaveCooker(const ConcaveCooker& t);
	ConcaveCooker& operator=(const ConcaveCooker& t);
};
