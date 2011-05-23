#pragma once
#include "D3DUtil.h"
#include <vector>

namespace IO{

typedef std::map<tstring, std::vector<std::pair<tstring, tstring>>> IniWriteData;

class IniWriter
{
public: 
    IniWriter();
    ~IniWriter();

    void Open(const std::string& path);
    void WriteInt(const std::tstring& root, const std::tstring& node, int i);
    void WriteFloat(const std::tstring& root, const std::tstring& node, float f);
    void WriteString(const std::tstring& root, const std::tstring& node, const tstring& str);
    bool Close();

private:
    IniWriteData m_Data;
    string m_Path;

    IniWriter& operator=(const IniWriter&) {};
    IniWriter(const IniWriter&) {};
};


} //end namespace
