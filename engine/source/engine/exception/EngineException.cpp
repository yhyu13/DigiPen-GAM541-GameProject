
#include "engine-precompiled-header.h"
#include "EngineException.h"

const std::wstring& gswy::EngineException::GetNote() const
{
    return note;
}

const std::wstring& gswy::EngineException::GetFile() const
{
    return file;
}

unsigned int gswy::EngineException::GetLine() const
{
    return line;
}

std::wstring gswy::EngineException::GetLocation() const
{
    return std::wstring( L"Line [" ) + std::to_wstring( line ) + L"] in " + file;
}

std::wstring gswy::EngineException::GetFullMessage() const
{
#if 1
    return GetNote() + L"\nAt: " + GetLocation();
#else
	return GetNote();
#endif
}

std::wstring gswy::EngineException::GetExceptionType() const
{
    return L"Engine Exception";
}

std::wstring gswy::str2wstr(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

std::string gswy::wstr2str(const std::wstring& wstr)
{
	return std::string(wstr.begin(), wstr.end());
}
