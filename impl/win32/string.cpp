#include <pch.h>
#include "../../interf/i_util.h"

#ifdef WIN32

#include <tuple>
#include <assert.h>
#include <algorithm>

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
	void enumPosPairs(const StringType& key, PosPairs& pairs)
	{
		//
		const UINT32 keySize = key.size();
		UINT32 startPos = 0;
		UINT32 pos = stub.find(key.str());
		while(pos>0){
			//
			pairs.push_back(std::make_pair(startPos, pos));
			startPos = pos + keySize;
			pos = stub.find(key.str(), startPos);
		}
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
	String::String(INT64 value, BaseSystem base /* = DECIMAL */)
	{
		char buffer[32] = {0};
		ex::BaseSystemConvert(value, base, buffer);
		stub = buffer;
	}
	String::String(UINT64 value, BaseSystem base /* = DECIMAL */)
	{
		char buffer[32] = {0};
		ex::BaseSystemConvert(value, base, buffer);
		stub = buffer;
	}
	String::~String()
	{
	}
// 	//////////////////////////////////////////////////////////////////////////
	UINT32 String::length() const
	{
		switch (stringCharset){
		case Charset::GBK:
			return GBK::length(stub.ptr());
		case Charset::UTF8:
			return UTF8::length(stub.ptr());		
			break;
		case Charset::UTF16:
			return UTF16::length(stub.wptr());
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
		return !stub.size();
	}
// 	Charset String::charset() const
// 	{
// 		return stringCharset;
// 	}
	const nChar* String::ptr() const
	{
		return stub.ptr();
	}
	const wChar* String::wptr() const
	{
		return stub.wptr();
	}
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
	void String::trim()
	{
		//
		if ( empty() )
		{
			return;
		}
	}
	void String::upperCase()
	{
		std::transform(
			stub.begin(), stub.end(), stub.begin()
			, [](char c)->int{return ::toupper(c);}
		);
	}
	void String::lowerCase()
	{
		std::transform(
			stub.begin(), stub.end(), stub.begin()
			, [](char c)->int{return ::tolower(c);}
		);
	}
	String&& String::toTrim() const
	{
		String str = *this;
		str.trim();
		return std::move(str);
	}
	String&& String::toUpperCase() const
	{
		String str = *this;
		str.upperCase();
		return std::move(str);
	}
	String&& String::toLowerCase() const
	{
		String str = *this;
		str.lowerCase();
		return std::move(str);
	}

	INT32 String::toINT32() const
	{
		//
		switch (stringCharset)
		{
		case Charset::GBK:
		case Charset::UTF8:
			return strtol(stub.ptr(), nullptr, (int)BaseSystem::DECIMAL);
			break;
		case Charset::UTF16:
		case Charset::UTF32:
			return wcstol(stub.wptr(), nullptr, (int)BaseSystem::DECIMAL);
			break;
		default:
			assert(0);
			return 0;
			break;
		}
	}
	UINT32 String::toUINT32() const
	{
		switch (stringCharset)
		{
		case Charset::GBK:
		case Charset::UTF8:
			return strtoul(stub.ptr(), nullptr, (int)BaseSystem::DECIMAL);
			break;
		case Charset::UTF16:
		case Charset::UTF32:
			return wcstoul(stub.wptr(), nullptr, (int)BaseSystem::DECIMAL);
			break;
		default:
			assert(0);
			return 0;
			break;
		}
	}

	INT64 String::toINT64() const
	{
		//
		switch (stringCharset)
		{
		case Charset::GBK:
		case Charset::UTF8:
			return GBK::toINT64(stub.ptr(), BaseSystem::DECIMAL);
			break;
		case Charset::UTF16:
		case Charset::UTF32:
			return UTF16::toINT64(stub.wptr(), BaseSystem::DECIMAL);
			break;
		default:
			assert(0);
			return 0;
			break;
		}
		return 0;
	}
	UINT64 String::toUINT64() const
	{
		switch (stringCharset)
		{
		case Charset::GBK:
		case Charset::UTF8:
			return GBK::toUINT64(stub.ptr(), BaseSystem::DECIMAL);
			break;
		case Charset::UTF16:
		case Charset::UTF32:
			return UTF16::toUINT64(stub.wptr(), BaseSystem::DECIMAL);
			break;
		default:
			assert(0);
			return 0;
			break;
		}
		return 0;
		return 0;
	}

	E_API bool replace(
		String&& from
		, String&& to
		, UINT32 offset = 0
		)
	{
		//
	}
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