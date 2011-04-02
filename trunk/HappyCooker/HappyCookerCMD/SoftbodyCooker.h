#pragma once
#include "stdafx.h"
#include <vector>
#include "Model.h"

class SoftbodyCooker
{
public:
    SoftbodyCooker(Model* pModel, const string& outputName);
	virtual ~SoftbodyCooker(void);

	virtual bool Cook();


private:
    string m_OutputFile;
	Model* m_pModel;

	SoftbodyCooker(const SoftbodyCooker& t);
	SoftbodyCooker& operator=(const SoftbodyCooker& t);
};
