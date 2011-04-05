#pragma once
#include "stdafx.h"
#include "Model.h"

class BinCooker
{
public:
    BinCooker(Model* pModel, const string output);
    ~BinCooker(void);

    bool Cook();

private:
    bool CookSB();
    bool CookNSB();

    Model* m_pModel;
    string m_OutputName;
};

