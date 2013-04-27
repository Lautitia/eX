#ifndef _EX_TYPES_H_
#define _EX_TYPES_H_

#include <vector>
#include <tuple>

typedef signed char		INT8;
typedef unsigned char	UINT8;

typedef signed short	INT16;
typedef unsigned short	UINT16;

typedef signed int		INT32;
typedef unsigned int	UINT32;

#ifdef __GNUC__
typedef long long			INT64;
typedef unsigned long long	UINT64;
#else
typedef signed __int64      INT64;
typedef unsigned __int64    UINT64;
#endif

typedef double		REAL;

#ifdef WIN32
typedef char	nChar;
typedef wchar_t wChar;

typedef std::vector<nChar> nCharArray;
typedef std::vector<wChar> wCharArray;

#endif

//Max Value Of Basic Variable Type
/*
EX32BIT/EX64BIT
*/
#ifdef EX32BIT
#define MAX_INT8 (((INT8)(~0))>>1)
#define MAX_UINT8 ((UINT8)(~0))

#define MAX_INT16 (((INT16)(~0))>>1)
#define MAX_UINT16 ((UINT16)(~0))

#define MAX_INT32 (((INT32)(~0))>>1)
#define MAX_UINT32 ((UINT32)(~0))

#define MAX_INT64 (((INT64)(~0))>>1)
#define MAX_UINT64 ((UINT64)(~0))
#endif

#ifdef EX64BIT
#endif

//
#define UTF8_1BYTE 0x00		//0XXX XXXX
#define UTF8_2BYTE 0xC0		//110X XXXX
#define UTF8_3BYTE 0xE0		//1110 XXXX
#define UTF8_4BYTE 0xF8		//1111 0XXX
#define UTF8_5BYTE 0xFC		//1111 10XX
#define UTF8_6BYTE 0xFE		//1111 110X

enum class BaseSystem{
	//
	BINARY = 2,
	OCTAL = 8,
	DECIMAL = 10,
	HEX = 16
};

/*
	Value of Charset is composed by follow rules
	CharsetIdentity(High 4bit) | ByteUsed(Low 4bit)
	e.g.
	GBK's CharsetIdentity = 0001
	GBK's use 1~2 Bytes it's MultiChar
	MultiChar = 0x0000
	WideChr = 0x0001
	So GBK enum value is 00010000
*/
enum class Charset{
	GBK = 0x10,		//00010000
	UTF8 = 0x20,	//00020000
	UTF16 = 0x31,	//00030001
	UTF32 = 0x41	//00040001
};

//CharsetOperation
template<Charset set, typename MethodT>
struct CharsetOperation
{
	typedef typename MethodT::Type Type;
	typedef std::tuple<const Type*, UINT32> SubTuple;

	static UINT32 length(const Type* pos)
	{
		if (pos == nullptr){
			return 0;
		}
		UINT32 size = 0;
		while(MethodT::move(&pos)){
			++size;
		}
		return size;
	}
	static SubTuple substr(const Type* str, bool isForward, UINT32 pos, UINT32 len)
	{
		//
		if (str == nullptr){
			return SubTuple(nullptr, 0);
		}

		const UINT32 strLen = length(str);
		if (!isForward){
			//get substr from the right(default is from left)
			const INT32 offsetPos = strLen - pos - len;
			if (offsetPos<0){
				//if pos+len > strLen pos is negetive
				//but it just happend in isForward=false
				len = strLen - pos;
				pos = 0;
			}else{
				pos = offsetPos;
			}
		}

		UINT32 size = 0;
		const Type* strPos = pos ? nullptr : str;
		while (*str && len){
			const UINT32 moved = MethodT::move(&str);
			size += moved;

			if (pos){
				--pos;
				size = 0;
				strPos = str;
			}
			else{
				len--;
			}
		}
		return SubTuple(strPos, size);
	}
};

//GBK
struct GBKMethod
{
	typedef nChar Type;
	static UINT32 move(const Type** pos)
	{
		//
		if (pos && **pos == '\0'){
			return 0;
		}
		UINT32 size = 0;
		UINT32 isGBK = **pos&0x80;
		if (isGBK){
			size = 2;
		}
		else{
			size = 1;
		}
		(*pos) += size;
		return size;
	}
};

//UTF8
struct UTF8Method
{
	typedef nChar Type;
	static int utf8bytes(char c)
	{
		//
		int size = 0;
		while(c&0x80){
			//
			c<<=1;
			size++;
		}
		if (!size){
			size = 1;
		}
		return size;
	}
	static UINT32 move(const Type** pos)
	{
		//
		if (pos && **pos == '\0'){
			return false;
		}
		UINT32 bytes = utf8bytes(**pos);
		(*pos) += bytes;
		return bytes;
	} 
};

//UTF16
struct UTF16Method
{
	typedef wChar Type;
	static UINT32 move(const Type** pos)
	{
		//
		if (pos && **pos == '\0'){
			return 0;
		}
		(*pos)++;
		return 1;
	}
};

typedef CharsetOperation<Charset::GBK, GBKMethod> GBK;
typedef CharsetOperation<Charset::UTF8, UTF8Method> UTF8;
typedef CharsetOperation<Charset::UTF16, UTF16Method> UTF16;

//CharsetConvert
template<typename from>
struct CharsetConvert;

//////////////////////////////////////////////////////////////////////////
bool IsWideCharset(Charset charSet)
{
	return ((int)charSet&0x0F == 1);
}
UINT32 CharsetIndex(Charset charSet)
{
	return ((int)charSet&0xF0>>4);
}

#endif