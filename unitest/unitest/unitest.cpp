// unitest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../eX.h"
#include <iostream>
#include <windows.h>

#include <tuple>
#include <functional>

static UINT32 utf82utf16(const nChar* src, int srcSize, wChar* dst)
{
	//
	UINT32 dstSize = MultiByteToWideChar(CP_UTF8, 0, src, srcSize, (LPWSTR)dst, 0);
	if ( dst != nullptr ){
		MultiByteToWideChar(CP_UTF8, 0, src, srcSize, (LPWSTR)dst, dstSize);
	}
	return dstSize;
}
static UINT32 utf162utf8(const wChar* src, int srcSize, nChar* dst)
{
	//
	UINT32 dstSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)src, srcSize, dst, 0, nullptr, nullptr);
	if( dst != nullptr ){
		WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)src, srcSize, dst, dstSize, nullptr, nullptr);
	}
	return dstSize;
}

//////////////////////////////////////////////////////////////////////////
//
template<>
struct CharsetMatchEncode<Charset::GBK, Charset::UTF8>
{
	typedef CharsetType<Charset::GBK>::Type TSrc;
	typedef CharsetType<Charset::UTF8>::Type TDest;
	static UINT32 MatchEncode(const TSrc* src, UINT32 srcSize, TDest* dest)
	{
		//GBK->UTF16
		UINT32 allocSize = MultiByteToWideChar(CP_ACP, 0, src, srcSize, nullptr, 0);
		wChar* wideBuffer = new wChar[allocSize];
		MultiByteToWideChar(CP_ACP, 0, src, srcSize, wideBuffer, allocSize);
		//UTF16->UTF8
		UINT32 dstSize = utf162utf8(wideBuffer, allocSize, dest);
		delete[] wideBuffer;
		wideBuffer = nullptr;
		return dstSize;
	}
};

template<typename T, T v>
struct A
{
	T value;
};
//////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	int size = 0;
	int pos = 0;

// 	{
// 		char buffer[] = "abc≤‚ ‘";
// 		std::string str = "abc≤‚ ‘";
// 		typedef CharsetSubStr<Charset::GBK> GBKSubStr;
// 		GBKSubStr::SubTuple subTuple = GBKSubStr::substr(buffer, false, 3, 9);
// 		int i = 0;
// 	}
// 	{
// 		char buffer[] = {'a', 'b', 'c', 'd', 0xE5, 0xBC, 0x80, 0xE5, 0x8F, 0x91, 0xe6 ,0xb5 ,0x8b ,0xe8 ,0xaf ,0x95, 0x00};
// 		typedef CharsetSubStr<Charset::UTF8> UTF8SubStr;
// 		UTF8SubStr::SubTuple subTuple = UTF8SubStr::substr(buffer, true, 4, 2);
// 		int i = 0;
// 	}
	{
// 		wChar buffer[] = L"abcdø™∑¢≤‚ ‘";
// 		typedef CharsetSubStr<Charset::UTF16> UTF16SubStr;
// 		UTF16SubStr::SubTuple tuple = UTF16SubStr::substr(buffer, true, 4, 2);
// 		int i = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	{
// 		char buffer[] = "abc≤‚ ‘";
// 		UINT32 gbkLen = sizeof(buffer)-1;
// 
// 		typedef CharsetMatchEncode<Charset::GBK, Charset::UTF8> GBK2UTF8;
// 		UINT32 dstSize = GBK2UTF8::MatchEncode(buffer, gbkLen, nullptr);
// 		char* destBuffer = new char[dstSize];
// 		GBK2UTF8::MatchEncode(buffer, gbkLen, destBuffer);
// 		delete[] destBuffer;
// 		destBuffer = nullptr;
	}
	Charset c = Charset::UTF8;
	A<decltype(Charset::GBK), c> structA;
	return 0;
}