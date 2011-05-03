#include "FileNotFoundException.h"

using namespace exceptions;

FileNotFoundException::FileNotFoundException(): m_Message(_T(""))
{
}
FileNotFoundException::FileNotFoundException(const tstring& filename): m_Message(_T("File ") + filename + _T(" not found!"))
{
}