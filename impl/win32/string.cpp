#include <pch.h>
#include "../../interf/i_util.h"

#ifdef WIN32

#include <tuple>
#include <assert.h>

namespace ex{
	static int utf82utf16(const nChar* src, int srcSize, wChar* dst)
	{
		//
		int dstSize = MultiByteToWideChar(CP_UTF8, 0, src, srcSize, (LPWSTR)dst, 0);
		if ( dst != nullptr ){
			MultiByteToWideChar(CP_UTF8, 0, src, srcSize, (LPWSTR)dst, dstSize);
		}
		return dstSize;
	}
	static int utf162utf8(const wChar* src, int srcSize, nChar* dst)
	{
		//
		int dstSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)src, srcSize, dst, 0, nullptr, nullptr);
		if( dst != nullptr ){
			WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)src, srcSize, dst, dstSize, nullptr, nullptr);
		}
		return dstSize;
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//
	String&& String::substr(bool isForward, UINT32 start, UINT32 len) const
	{
		switch (stringCharset)
		{
		case Charset::GBK:
			{
				GBKSubStr::SubTuple tuple = GBKSubStr::substr(nString.c_str(), isForward, start, len);
				return std::move(
					String(std::get<0>(tuple), stringCharset, std::get<1>(tuple))
					);
			}			
			break;
		case Charset::UTF8:
			{
				UTF8SubStr::SubTuple tuple = UTF8SubStr::substr(nString.c_str(), isForward, start, len);
				return std::move(
					String(std::get<0>(tuple), stringCharset, std::get<1>(tuple))
					);
			}
			break;
		case Charset::UTF16:
			{
				UTF16SubStr::SubTuple tuple = UTF16SubStr::substr(wString.c_str(), isForward, start, len);
				return std::move(
					String(std::get<0>(tuple), stringCharset, std::get<1>(tuple))
					);
			}
			break;
		case Charset::UTF32:
			break;
		default:
			break;
		}
		return std::move(String());
	}
	//////////////////////////////////////////////////////////////////////////
	String::String()
		: stringCharset(Charset::GBK)
	{}
	String::String(const nChar* str, Charset charset/* = Charset::GBK*/, UINT32 size /* = MAX_UINT32 */)
	{
		//
		if (size == MAX_UINT32){
			nString = std::string(str);
		}
		else{
			nString = std::string(str, size);
		}
		stringCharset = charset;
	}
	String::String(const wChar* str, Charset charset/* = Charset::UTF16*/, UINT32 size /* = MAX_UINT32 */)
	{
		//
		if (size == MAX_UINT32){
			wString = std::wstring(str);
		}
		else{
			wString = std::wstring(str, size);
		}
		stringCharset = charset;
	}

	String::String(const String& str)
	{
		stringCharset = str.stringCharset;
		nString = str.nString;
		wString = str.wString;
	}
	String::String(INT64 value, BaseSystem base /* = DECIMAL */)
	{
		char buffer[32] = {0};
		ex::BaseSystemConvert(value, base, buffer);
		nString = buffer;
	}
	String::String(UINT64 value, BaseSystem base /* = DECIMAL */)
	{
		char buffer[32] = {0};
		ex::BaseSystemConvert(value, base, buffer);
		nString = buffer;
	}
	String::~String()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	UINT32 String::length() const
	{
		switch (stringCharset){
		case Charset::GBK:
			return GBKLength::length(nString.c_str());
		case Charset::UTF8:
			return UTF8Length::length(nString.c_str());		
			break;
		case Charset::UTF16:
			return UTF16Length::length(wString.c_str());
			break;
		case Charset::UTF32:
			//Now it isn't Support 
			assert(0);
			break;
		default:
			assert(0);
			break;
		}
		return 0;
	}
	bool String::empty() const
	{
		if (IsWideCharset(stringCharset)){
			//
			return !nString.length();
		}
		else{
			//
			return !wString.length();
		}
	}
	Charset String::charset() const
	{
		return stringCharset;
	}
	//////////////////////////////////////////////////////////////////////////
	String&& String::left(UINT32 len) const
	{
		return substr(true, 0, len);
	}
	String&& String::right(UINT32 len) const
	{
		return substr(false, 0, len);
	}
	String&& String::mid(UINT32 start, UINT32 len/* = MAX_UINT32*/) const
	{
		return substr(true, start, len);
	}
	//////////////////////////////////////////////////////////////////////////
}

#endif