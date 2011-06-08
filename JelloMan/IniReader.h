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

    bool IsOpen() const { return m_IsOpen; }

    int GetInt(const tstring& root, const tstring& node) const;
    float GetFloat(const tstring& root, const tstring& node) const; 
    tstring GetString(const tstring& root, const tstring& node) const;

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