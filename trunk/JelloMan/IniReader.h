#pragma once
#include "D3DUtil.h"
#include <map>

namespace IO{

typedef map<tstring, map<tstring, tstring>> IniReadData;

class IniReader
{
public:
    IniReader();
    ~IniReader();

    void Open(string path);

    bool IsOpen() { return m_IsOpen; }

    int GetInt(const tstring& root, const tstring& node);
    float GetFloat(const tstring& root, const tstring& node); 
    tstring GetString(const tstring& root, const tstring& node);

private:
    IniReadData m_Data;
    bool m_IsOpen;

    IniReader& operator=(const IniReader&) {};
    IniReader(const IniReader&) {};
};

class RootNotFoundException {};
class NodeNotFoundException {};
class ParseFailException {};


} //end namespace