#include "ProgressInfo.h"
#include "stdafx.h"

ProgressInfo::ProgressInfo(void): m_Max(0), m_Ticks(0)
{
}


ProgressInfo::~ProgressInfo(void)
{
}

void ProgressInfo::SetMax(int max)
{
    m_Max = max;
}
void ProgressInfo::AddTick()
{
    BYTE prev = GetProgress(); 
    ++m_Ticks;
    if (prev < GetProgress())
        cout << ".";
}
void ProgressInfo::AddTick(int ticks)
{
    BYTE prev = GetProgress(); 
    m_Ticks += ticks;
    BYTE curr = GetProgress();
    for (int i = 0; i < curr  - prev; ++i)
        cout << ".";
}

BYTE ProgressInfo::GetProgress()
{
    return static_cast<unsigned char>(m_Ticks / static_cast<float>(m_Max) * 100);
}