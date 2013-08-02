#include <pch.h>
#include "../../interf/i_util.h"

#ifdef WIN32

#include <tuple>
#include <assert.h>

namespace ex{
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//
	String&& String::substr(bool isForward, UINT32 start, UINT32 len) const
	{
		switch (stringCharset)
		{
		case Charset::GBK:
			{
				GBK::SubTuple tuple = GBK::substr(stub.ptr(), isForward, start, len);
				return std::move(
					String(std::get<0>(tuple), std::get<1>(tuple))
					);
			}			
			break;
		case Charset::UTF8:
			{
				UTF8::SubTuple tuple = UTF8::substr(stub.ptr(), isForward, start, len);
				return std::move(
					String(std::get<0>(tuple), std::get<1>(tuple))
					);
			}
			break;
		case Charset::UTF16:
			{
				UTF16::SubTuple tuple = UTF16::substr(stub.wptr(), isForward, start, len);
				return std::move(
					String(std::get<0>(tuple), std::get<1>(tuple))
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
		: stringCharset(EX_DEFAULT_CHARSET)
	{}
	String::String(const nChar* str, Charset charset)
		: stringCharset(charset)
		, stub(str)
	{}
	String::String(const nChar* str, UINT32 size, Charset charset)
		: stringCharset(charset)
		, stub(str, size)
	{}
	String::String(const wChar* str, Charset charset)
		: stringCharset(charset)
		, stub(str)
	{}
	String::String(const wChar* str, UINT32 size, Charset charset)
		: stringCharset(charset)
		, stub(str, size)
	{}
	 
 	String::String(const String& str)
 	{
		stringCharset = str.stringCharset;
		stub = str.stub;
 	}
// 	String::String(INT64 value, BaseSystem base /* = DECIMAL */)
// 	{
// 		char buffer[32] = {0};
// 		ex::BaseSystemConvert(value, base, buffer);
// 		nString = buffer;
// 	}
// 	String::String(UINT64 value, BaseSystem base /* = DECIMAL */)
// 	{
// 		char buffer[32] = {0};
// 		ex::BaseSystemConvert(value, base, buffer);
// 		nString = buffer;
// 	}
// 	String::~String()
// 	{
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	UINT32 String::length() const
// 	{
// 		switch (stringCharset){
// 		case Charset::GBK:
// 			return GBK::length(nString.c_str());
// 		case Charset::UTF8:
// 			return UTF8::length(nString.c_str());		
// 			break;
// 		case Charset::UTF16:
// 			return UTF16::length(wString.c_str());
// 			break;
// 		case Charset::UTF32:
// 			//Now it isn't Support 
// 			assert(0);
// 			break;
// 		default:
// 			assert(0);
// 			break;
// 		}
// 		return 0;
// 	}
// 	bool String::empty() const
// 	{
// 		if (IsWideCharset(stringCharset)){
// 			//
// 			return !nString.length();
// 		}
// 		else{
// 			//
// 			return !wString.length();
// 		}
// 	}
// 	Charset String::charset() const
// 	{
// 		return stringCharset;
// 	}
// 	nChar String::nstr() const
// 	{
// 		return nString.c_str();
// 	}
// 	wChar String::wstr() const
// 	{
// 		return wString.c_str();
// 	}
// 	//////////////////////////////////////////////////////////////////////////
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
// 	//////////////////////////////////////////////////////////////////////////
// 	String&& String::toGBK() const
// 	{
// 		switch (stringCharset)
// 		{
// 		case Charset::GBK:
// 			break;
// 		case Charset::UTF8:
// 			break;
// 		case Charset::UTF16:
// 			break;
// 		case Charset::UTF32:
// 			break;
// 		default:
// 			break;
// 		}
// 	}

#ifdef WIN32
	UINT32 gbk2utf16(const nChar* src, UINT32 srcSize, wChar* dst)
	{
		//
		int dstSize = MultiByteToWideChar(CP_ACP, 0, src, srcSize, (LPWSTR)dst, 0);
		if ( dst != nullptr ){
			MultiByteToWideChar(CP_ACP, 0, src, srcSize, (LPWSTR)dst, dstSize);
		}
		return dstSize;
	}
	UINT32 utf162gbk(const wChar* src, UINT32 srcSize, nChar* dst)
	{
		int dstSize = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)src, srcSize, dst, 0, nullptr, nullptr);
		if( dst != nullptr ){
			WideCharToMultiByte(CP_ACP, 0, (LPCWCH)src, srcSize, dst, dstSize, nullptr, nullptr);
		}
		return dstSize;
	}
	UINT32 utf82utf16(const nChar* src, UINT32 srcSize, wChar* dst)
	{
		//
		int dstSize = MultiByteToWideChar(CP_UTF8, 0, src, srcSize, (LPWSTR)dst, 0);
		if ( dst != nullptr ){
			MultiByteToWideChar(CP_UTF8, 0, src, srcSize, (LPWSTR)dst, dstSize);
		}
		return dstSize;
	}
	UINT32 utf162utf8(const wChar* src, UINT32 srcSize, nChar* dst)
	{
		//
		int dstSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)src, srcSize, dst, 0, nullptr, nullptr);
		if( dst != nullptr ){
			WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)src, srcSize, dst, dstSize, nullptr, nullptr);
		}
		return dstSize;
	}
	UINT32 gbk2utf8(const nChar* src, UINT32 srcSize, nChar* dest)
	{
		//GBK->UTF16
		UINT32 allocSize = gbk2utf16(src, srcSize, nullptr);
		wChar* wideBuffer = new wChar[allocSize];
		gbk2utf16(src, srcSize, wideBuffer);
		//UTF16->UTF8
		UINT32 dstSize = utf162utf8(wideBuffer, allocSize, dest);
		delete[] wideBuffer;
		wideBuffer = nullptr;
		return dstSize;
	}
	UINT32 utf82gbk(const nChar* src, UINT32 srcSize, nChar* dest)
	{
		//utf8->utf16
		UINT32 allocSize = utf82utf16(src, srcSize, nullptr);
		wChar* wideBuffer = new wChar[allocSize];
		utf82utf16(src, srcSize, wideBuffer);
		//UTF16->UTF8
		UINT32 dstSize = utf162gbk(wideBuffer, allocSize, dest);
		delete[] wideBuffer;
		wideBuffer = nullptr;
		return dstSize;
	}
#endif
}

#endif