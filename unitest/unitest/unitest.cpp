// unitest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../eX.h"
#include <iostream>
#include <windows.h>

#include <tuple>
#include <functional>

#include "string.h"

bool stringstub_testcase()
{
	//
	using namespace std;

	cout<<"begin test with nChar"<<endl;
	ex::StringStub ss("≤‚ ‘");
	if( !(ss == "≤‚ ‘") )
	{
		cout<<"operator == | constructor failed"<<endl;
		return false;
	}

	ss = ss+"456";
	if( !(ss == "≤‚ ‘456") )
	{
		cout<<"operator + failed"<<endl;
		return false;
	}

	ss += "abc";
	if( !(ss == "≤‚ ‘456abc") )
	{
		cout<<"operator += failed"<<endl;
		return false;
	}

	{
		//
		ex::StringStub outs;
		outs = ss;

		if( !(outs == "≤‚ ‘456abc") )
		{
			cout<<"operator = failed"<<endl;
			return false;
		}
	}
	
	///
//	cout<<"begin test with wChar"<<endl;

	return true;
}

//////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
// 	int size = 0;
// 	int pos = 0;

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

	/*
	if( !utest::string::run() ){
		//
	}
	*/

	/*
	ex::StringStub ss = L"Œ“√«¿¥ ‘œ¬";

	wChar* p = (wChar*)ss.ptr();

	std::wstring str = (wchar_t*)p;

	{
		//
		ex::StringStub v = "123";
		v = v+ "456";
		std::cout<<v.ptr()<<std::endl;
	}
	*/
	assert(stringstub_testcase());

	return 0;
}