#include "IniWriter.h"

using namespace std;

namespace IO{

IniWriter::IniWriter()
{
}
IniWriter::~IniWriter()
{
    if (m_Path != "")
        Close();
}
void IniWriter::Open(const std::string& path)
{
    m_Path = path;
    m_Data.clear();
}
void IniWriter::WriteInt(const std::tstring& root, const std::tstring& node, int i)
{
    tstringstream str;
    str << i;
    WriteString(root, node, str.str());
}
void IniWriter::WriteFloat(const std::tstring& root, const std::tstring& node, float f)
{
    tstringstream str;
    str << f;
    WriteString(root, node, str.str());
}
void IniWriter::WriteString(const std::tstring& root, const std::tstring& node, const tstring& str)
{
    m_Data[root].push_back(make_pair(node, str));
}
bool IniWriter::Close()
{
    tofstream stream(m_Path.c_str());
    m_Path = "";

    if (stream.is_open())
    {
        for_each(m_Data.cbegin(), m_Data.cend(), [&stream](const pair<tstring, vector<pair<tstring, tstring>>>& root)
        {
            stream << "[" << root.first << "]" << "\n";

            tofstream& innerstream = stream; //nested lambda loses scope --> workaround
            for_each(root.second.cbegin(), root.second.cend(), [&innerstream](const pair<tstring, tstring>& node)
            {
                innerstream << node.first << " = " << node.second << "\n";
            });
            stream << "\n";
        });
        stream.close();
        return true;
    }
    else
    {
        return false;
    }
}


} //end namespace
