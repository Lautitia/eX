#ifndef _I_EX_H_
#define _I_EX_H_

#ifdef WIN32
#include <windows.h>
#endif

#include <vector>
#include <assert.h>

namespace ex{

	typedef std::vector<char> CharStub;
	class StringStub
	{
	private:
		CharStub stub;
		static const UINT32 stringEndLength = sizeof(wChar);

	public:
		StringStub()
		{}
		StringStub(const nChar*value, UINT32 size)
		{
			buildWithCharT(value, size, false);
		}
		StringStub(const nChar* value)
		{
			this->operator=(value);
		}
		StringStub(const wChar* value, UINT32 size)
		{
			buildWithCharT(value, size, false);
		}
		StringStub(const wChar* value)
		{
			this->operator=(value);
		}
		virtual ~StringStub()
		{}

	public:
		UINT32 size() const
		{
			if(!stub.size())
			{
				return 0;
			}
			else
			{
				return stub.size() - stringEndLength;
			}
		}
		const nChar* ptr() const
		{
			return &stub[0];
		}
		const wChar* wptr() const
		{
			return (wChar*)&stub[0];
		}
		CharStub::iterator begin()
		{
			return stub.begin();
		}
		CharStub::iterator end()
		{
			return stub.end();
		}

	public:
		UINT32 find(const nChar* str)
		{
			return 0;
		}
		UINT32 find(const wChar* str)
		{
			return 0;
		}

	public:
		StringStub& operator +(const StringStub& value)
		{
			//
			CharStub tempBuffer;
			tempBuffer.resize(size() + value.size() + stringEndLength, 0);
			memcpy((void*)&tempBuffer[0], (const void*)ptr(), size());
			memcpy((void*)&tempBuffer[size()], (const void*)value.ptr(), value.size());
			stub = tempBuffer;
			return *this;
		}
		StringStub& operator +(const nChar* value)
		{
			//
			buildWithCharT(value, MAX_UINT32, true);
			return *this;
		}
		StringStub& operator +(const wChar* value)
		{
			//
			buildWithCharT(value, MAX_UINT32, true);
			return *this;
		}


		StringStub& operator +=(const StringStub& value)
		{
			return this->operator+(value);
		}
		StringStub& operator +=(const nChar* value)
		{
			return this->operator+(value);
		}
		StringStub& operator +=(const wChar* value)
		{
			return this->operator+(value);
		}

		StringStub& operator =(const nChar* value)
		{
			//
			buildWithCharT(value, MAX_UINT32, false);
			return *this;
		}
		StringStub& operator =(const wChar* value)
		{
			buildWithCharT(value, MAX_UINT32, false);
			return *this;
		}

		/*
		we can't distinguish the [] return nChar or wChar
		const nChar& operator [](UINT32 index) const
		{
			return stub[index];
		}
		*/

		bool operator ==(const nChar* value)
		{
			const int ret = memcmp((const void*)&stub[0], (const void*)value, size());
			return ret == 0;
			//return strcmp((const char*)&stub[0], value);
		}
		bool operator ==(const wChar* value)
		{
			const int ret = memcmp((const void*)&stub[0], (const void*)value, size());
			return ret == 0;
		}

	protected:
		template<typename T>
		void buildWithCharT(const T* value, UINT32 size, bool isAppend)
		{
			//
			CharStub tempBuffer;
			if(isAppend)
			{
				tempBuffer = stub;
			}
			for(;*value != 0 && size;value++, size--)
			{
				char* v = (char*)value;
				for(int i=0; i<sizeof(T); i++, v++)
				{
					//
					tempBuffer.push_back( *v );
				}
			}
			fillEndOfString();
			stub = tempBuffer;
		}
		void fillEndOfString()
		{
			for (int i=0; i<stringEndLength; i++)
			{
				stub.push_back('\0');
			}
		}
	};

	/////////////////////////////////////////////////////////////////////////////
	//
	class String;
	typedef std::vector<String> StringArray;
	class String
	{
	private:
		Charset stringCharset;
		StringStub stub;

	private:
		String&& substr(bool isForward, UINT32 start, UINT32 len) const;
		String&& convert(Charset convertTo);

	public:
		E_API String();
		E_API String(const nChar* str, Charset charset = EX_DEFAULT_CHARSET);
		E_API String(const nChar* str, UINT32 size = MAX_UINT32, Charset charset = EX_DEFAULT_CHARSET);
		E_API String(const wChar* str, Charset charset = EX_DEFAULT_CHARSET);
		E_API String(const wChar* str, UINT32 size = MAX_UINT32, Charset charset = EX_DEFAULT_CHARSET);
		
		E_API String(const String& str);

		E_API String(INT64 value, BaseSystem base = BaseSystem::DECIMAL);
		E_API String(UINT64 value, BaseSystem base = BaseSystem::DECIMAL);
		
		virtual ~String();

		E_API UINT32 length() const;
		E_API bool empty() const;
//		E_API Charset charset() const;
		E_API const nChar* ptr() const;
		E_API const wChar* wptr() const;

	public:
		E_API String&& left(UINT32 len) const;
		E_API String&& right(UINT32 len) const;
		E_API String&& mid(UINT32 start, UINT32 len = MAX_UINT32) const;

		E_API void trim();
		E_API void upperCase();
		E_API void lowerCase();
		E_API String&& toTrim() const;
		E_API String&& toUpperCase() const;
		E_API String&& toLowerCase() const;

	public:
		E_API INT32 toINT32() const;
		E_API UINT32 toUINT32() const;

		E_API INT64 toINT64() const;
		E_API UINT64 toUINT64() const;
		
		E_API bool replace(
			String&& from
			, String&& to
			, UINT32 offset = 0
			);

		E_API StringArray&& split(
			String&& splitor
			,bool caseSensitive = false
			);

		E_API UINT32 find(
			String&& key
			, UINT32 offset = 0
			, bool fromBegin = true
			, bool caseSensitive = false
			);
	};

#ifdef WIN32
	UINT32 gbk2utf16(const nChar* src, UINT32 srcSize, wChar* dst);
	UINT32 utf162gbk(const wChar* src, UINT32 srcSize, nChar* dst);
	UINT32 utf82utf16(const nChar* src, UINT32 srcSize, wChar* dst);
	UINT32 utf162utf8(const wChar* src, UINT32 srcSize, nChar* dst);
	UINT32 gbk2utf8(const nChar* src, UINT32 srcSize, nChar* dest);
	UINT32 utf82gbk(const nChar* src, UINT32 srcSize, nChar* dest);
#endif

	///////////////////////////////////////////////////////////////////
	//Charset Convert
	/*
	template<Charset from, Charset to>
	struct CharsetConvert
	{
	};
	*/

	template<typename T>
	class StringT
	{
	public:
		typedef StringT<T> StringType;
		typedef std::vector<StringT<T>> StringTArray;
		typedef std::pair<UINT32, UINT32> PosPair;
		typedef std::vector<PosPair> PosPairs;

	private:
		Charset stringCharset;
		typedef std::basic_string<T, std::char_traits<T>, std::allocator<T> > String;
		String stub;

	private:
		StringT<T>&& substr(bool isForward, UINT32 start, UINT32 len) const
		{
			switch (stringCharset)
			{
			case Charset::GBK:
				{
					GBK::SubTuple tuple = GBK::substr(nString.c_str(), isForward, start, len);
					return std::move(
						String(std::get<0>(tuple), stringCharset, std::get<1>(tuple))
						);
				}			
				break;
			case Charset::UTF8:
				{
					UTF8::SubTuple tuple = UTF8::substr(nString.c_str(), isForward, start, len);
					return std::move(
						String(std::get<0>(tuple), stringCharset, std::get<1>(tuple))
						);
				}
				break;
			case Charset::UTF16:
				{
					UTF16::SubTuple tuple = UTF16::substr(wString.c_str(), isForward, start, len);
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
#ifdef WIN32
		template<typename from, typename to>
		UINT32 convert(Charset charset, typename const from::Type* src, UINT32 srcSize, typename to::Type* dest)
		{
			if (stringCharset == charset){
				return 0;
			}
			switch (stringCharset)
			{
			case Charset::GBK:
				{
					switch (charset)
					{
					case Charset::UTF8:
						return gbk2utf8(src, srcSize, dest);
						break;
					case Charset::UTF16:
						return gbk2utf16(src, srcSize, dest);
						break;
					case Charset::UTF32:
						break;
					default:
						break;
					}
				}
				break;
			case Charset::UTF8:
				switch (charset)
				{
				case Charset::GBK:
					return utf82gbk(src, srcSize, dest);
					break;
				case Charset::UTF8:
					break;
				case Charset::UTF16:
					return utf82utf16(src, srcSize, dest);
					break;
				case Charset::UTF32:
					break;
				default:
					break;
				}
				break;
			case Charset::UTF16:
				switch (charset)
				{
				case Charset::GBK:
					return utf162gbk(src, srcSize, dest);
					break;
				case Charset::UTF8:
					return utf162utf8(src, srcSize, dest);
					break;
				case Charset::UTF16:
					break;
				case Charset::UTF32:
					break;
				default:
					break;
				}
				break;
			case Charset::UTF32:
				break;
			default:
				break;
			}
		}
#endif
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

	public:
		E_API StringT()
			: stringCharset(Charset::GBK)
		{}
#ifdef WIN32
		E_API StringT(const nChar* str, Charset charset = Charset::GBK, UINT32 size = MAX_UINT32)
		{
			//
			if (size == MAX_UINT32){
				stub = String(str);
			}
			else{
				stub = String(str, size);
			}
			stringCharset = charset;
		}
#endif // WIN32

#ifdef WIN32
		E_API StringT(const wChar* str, Charset charset = Charset::UTF16, UINT32 size = MAX_UINT32)
		{
			//default narrow charset is GBK under win32
		}
#endif // WIN32

		E_API StringT(const StringT<nChar>& str)
		{
			stringCharset = str.stringCharset;
			stub = str.stub;
		}
		E_API StringT(const StringT<wChar>& str)
		{
			//
		}

		E_API StringT(INT64 value, BaseSystem base = BaseSystem::DECIMAL)
		{
			char buffer[32] = {0};
			ex::BaseSystemConvert(value, base, buffer);
			nString = buffer;
		}
		E_API StringT(UINT64 value, BaseSystem base = BaseSystem::DECIMAL)
		{
			char buffer[32] = {0};
			ex::BaseSystemConvert(value, base, buffer);
			nString = buffer;
		}

		virtual ~StringT()
		{}

		E_API UINT32 length() const
		{
			switch (stringCharset)
			{
			case Charset::GBK:
				return GBK::length(stub.c_str());
				break;
			case Charset::UTF8:
				return UTF8::length(stub.c_str());
				break;
			case Charset::UTF16:
				return UTF16::length(stub.c_str());
				break;
			case Charset::UTF32:
				break;
			default:
				assert(0);
				break;
			}
			return 0;
		}
		E_API UINT32 size() const
		{
			return stub.size();
		}
		E_API bool empty() const
		{
			return !stub.length();
		}
		E_API Charset charset() const
		{
			return stringCharset;
		}
		E_API const T* str() const
		{
			return stub.c_str();
		}

	public:
		E_API StringT<T>&& left(UINT32 len) const
		{
			return substr(true, 0, len);
		}
		E_API StringT<T>&& right(UINT32 len) const
		{
			return substr(false, 0, len);
		}
		E_API StringT<T>&& mid(UINT32 start, UINT32 len = MAX_UINT32) const
		{
			return substr(true, start, len);
		}

		E_API void trim()
		{
			if (!stub.length()){
				return;
			}
			T* begin = stub.c_str();
			while(*begin != ' '){
				++begin;
			}
			T* end = stub[stub.length()-sizeof(T)];
			while(*end != ' '){
				--end;
			}
			stub = StringT<T>(begin, stringCharset, abs(end-begin));
		}
		E_API void upperCase()
		{
			std::transform(stub.begin(), stub.end(), stub.begin(), ::toupper());
		}
		E_API void lowerCase()
		{
			std::transform(stub.begin(), stub.end(), stub.begin(), ::tolower());
		}
		E_API StringT<T>&& toTrim() const
		{
			StringT<T> str = *this;
			str.trim();
			return std::move(str);
		}
		E_API StringT<T>&& toUpperCase() const
		{
			StringT<T> str = *this;
			str.upperCase();
			return std::move(str);
		}
		E_API StringT<T>&& toLowerCase() const
		{
			StringT<T> str = *this;
			str.lowerCase();
			return std::move(str);
		}

	public:
// 		E_API INT32 toINT32(BaseSystem base = BaseSystem::DECIMAL) const
// 		{
// 			//			
// 		}
// 		E_API UINT32 toUINT32(BaseSystem base = BaseSystem::DECIMAL) const;
// 
// 		E_API INT64 toINT64(BaseSystem base = BaseSystem::DECIMAL) const;
// 		E_API UINT64 toUINT64(BaseSystem base = BaseSystem::DECIMAL) const;

// 		E_API void MatchEncode(const StringT<T>& matchTo)
// 		{
// 			//
// 			UINT32 allocSize = convert<decltype>(matchTo.charset(), stub.c_str(), stub.size(), nullptr);
// 			if (!allocSize){
// 				return;
// 			}
// 			String str;
// 			str.resize(allocSize);
// 			convert(matchTo.charset(), stub.c_str(), stub.size(), &str[0]);
// 		}
// 		E_API StringT<T>&& toGBK() const
// 		{
// 			UINT32 allocSize = convert(Charset::GBK, stub.c_str(), stub.size(), nullptr);
// 			String str;
// 			str.resize(allocSize);
// 			convert(Charset::GBK, stub.c_str(), stub.length(), &str[0]);
// 			return std::move(str);
// 		}
// 		E_API StringT<T>&& toUTF8() const
// 		{
// 			UINT32 allocSize = convert(Charset::UTF8, stub.c_str(), stub.size(), nullptr);
// 			String str;
// 			str.resize(allocSize);
// 			convert(Charset::UTF8, stub.c_str(), stub.length(), &str[0]);
// 			return std::move(str);
// 		}
// 		E_API StringT<T>&& toUTF16() const
// 		{
// 			UINT32 allocSize = convert(Charset::UTF16, stub.c_str(), stub.size(), nullptr);
// 			String str;
// 			str.resize(allocSize);
// 			convert(Charset::UTF16, stub.c_str(), stub.length(), &str[0]);
// 			return std::move(str);
// 		}

		E_API bool replace(
			StringT<T>&& from
			, StringT<T>&& to
			, UINT32 offset = 0
			)
		{
// 			from.MatchEncode(*this);
// 			to.MatchEncode(*this);

			PosPairs pairs;
			enumPosPairs(from, pairs);
			const UINT32 fromLen = pairs.size() * from.size();
			const UINT32 toLen = pairs.size() * to.size();
			const UINT32 newLen = stub.size() - fromLen + toLen;
			String str;
			str.resize(newLen, 0);
			UINT32 pos = 0;
			for each (auto& v in pairs){
				const UINT32 len = v.second-v.first+1;
				memcpy(str[pos], stub.c_str()+v.first, len);
				pos += len;
				memcpy(str[pos], to.str(), to.size());
				pos += to.size();
			}
			stub = str;
			return true;
		}

		E_API StringTArray&& split(
			StringT<T>&& splitor
			)
		{
// 			splitor.MatchEncode(*this);

			StringTArray array;
			PosPairs pairs;
			enumPosPairs(splitor, pairs);
			for each (auto& v in pairs)
			{
				array.push_back( StringType(stub.c_str()+v.first, v.second-v.first+1) );
			}
			return std::move(array);
		}

		E_API UINT32 find(
			StringT<T>&& key
			, UINT32 offset = 0
			)
		{
// 			key.MatchEncode(*this);
			return stub.find(key.str(), offset);
		}

		E_API T* getBuffer(UINT32 size)
		{
			//
			stub.resize(size);
			return &stub[0];
		}
	};
	typedef StringT<nChar> NString;
	typedef StringT<wChar> WString;
}

#endif