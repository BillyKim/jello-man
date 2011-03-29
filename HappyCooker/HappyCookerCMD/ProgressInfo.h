#pragma once
class ProgressInfo
{
public:
    ProgressInfo(void);
    ~ProgressInfo(void);

    void SetMax(int max);
    void AddTick();
    void AddTick(int ticks);

    unsigned char GetProgress();

private:
    int m_Max;
    int m_Ticks;
};

