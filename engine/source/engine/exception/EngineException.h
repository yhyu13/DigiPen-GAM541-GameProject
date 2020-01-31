/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 01/28/2020
- End Header ----------------------------*/

#pragma once
#include "engine/EngineCore.h"
#include <string>

namespace gswy
{
	class ENGINE_API EngineException
	{
	public:
		EngineException() = delete;
		explicit EngineException( const wchar_t* file,unsigned int line,const std::wstring& note = L"" )
			:
			file( file ), //_CRT_WIDE(__FILE__)
			line( line ), //__LINE__,
			note( note )  //L"Error!"
		{}
		const std::wstring& GetNote() const;
		const std::wstring& GetFile() const;
		unsigned int GetLine() const;
		std::wstring GetLocation() const;
		virtual std::wstring GetFullMessage() const;
		virtual std::wstring GetExceptionType() const;
	private:
		std::wstring note;
		std::wstring file;
		unsigned int line;
	};

	std::wstring str2wstr(const std::string& str);

	std::string wstr2str(const std::wstring& wstr);
}
