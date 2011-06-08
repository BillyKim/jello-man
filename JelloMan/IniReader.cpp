#include "IniReader.h"
#include "Exception.h"


using namespace std;

namespace IO{


bool ParseKeyValue(const tstring& line, tstring& key, tstring& value)
{
    tstringstream streamKey, streamValue;
    bool bKey = true;

    for_each(line.cbegin(), line.cend(), [&](tchar c)
    {
        if (bKey == true)
        {
            if (c == '=')
                bKey = false;
            else
                streamKey << c; 
        }
        else
        {
            streamValue << c;
        }
    });

    if (bKey == true)
        return false;

    key = streamKey.str();
    value = streamValue.str();

    return true;
}
tstring GetSubDivision(const tstring& str)
{
    return str.substr(1, str.size() - 2);
}
void RemoveSpaces(tstring& str)
{
    tstringstream stream;
    for_each(str.cbegin(), str.cend(), [&](tchar c)
    {
        if (c != ' ')
            stream << c;
    });
    str = stream.str();
}


IniReader::IniReader(): m_IsOpen(false)
{
}
IniReader::~IniReader()
{
}
void IniReader::Open(string path)
{
    m_Data.clear();

    tifstream file;
    file.open(path, ios::in);
    
    if (file.is_open())
    {
        map<tstring, tstring> subData;
        tstring line;
        tstring sub = _T("");
        while(file.eof() == false)
        {
            getline(file, line);
            RemoveSpaces(line);
            if (line.size() != 0)
            {
                if (line[0] == '[')
                {
                    if (sub != _T(""))
                        m_Data.insert(make_pair(sub, subData));
                    sub = GetSubDivision(line);
                    subData = map<tstring, tstring>();
                }
                else
                {
                    tstring key, value;
                    if (ParseKeyValue(line, key, value))
                        subData.insert(make_pair(key, value));
                }
            }
        }
    
        if (sub != _T(""))
            m_Data.insert(make_pair(sub, subData));
        file.close();
    }
    else
    {
        throw exceptions::FileNotFoundException();
    }
    m_IsOpen = true;
}

int IniReader::GetInt(const tstring& root, const tstring& node) const
{
    tstring temp(GetString(root, node));
    string fString(temp.cbegin(), temp.cend());
    int ret = MAXINT;
    if  (sscanf_s(fString.c_str(), "%d", &ret) == EOF || ret == MAXINT)
        throw ParseFailException();
    return ret;
}
float IniReader::GetFloat(const tstring& root, const tstring& node) const
{
    tstring temp(GetString(root, node));
    string fString(temp.cbegin(), temp.cend());
    float ret = 0;
    if  (sscanf_s(fString.c_str(), "%f", &ret) == EOF)
        throw ParseFailException();
    return ret;
}
tstring IniReader::GetString(const tstring& root, const tstring& node) const
{
    IniReadData::const_iterator itRoot = m_Data.find(root);
    if (itRoot != m_Data.cend())
    {
        map<tstring, tstring>::const_iterator itNode = itRoot->second.find(node);
        if (itNode != itRoot->second.cend())
        {
            return itNode->second;
        }
        else
        {
            throw NodeNotFoundException();
        }
    }
    else
    {
        throw RootNotFoundException();
    }
}

} //end namespace