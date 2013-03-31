#ifndef _I_EX_H_
#define _I_EX_H_

#include <vector>

namespace ex{
	class String;
	typedef std::vector<String> StringArray;
	class String
	{
	private:
		Charset stringCharset;
		std::string nString;
		std::wstring wString;

	private:
		String&& substr(bool isForward, UINT32 start, UINT32 len) const;

	public:
		E_API String();
#ifdef WIN32
		E_API String(const nChar* str, Charset charset = Charset::GBK, UINT32 size = MAX_UINT32);
#endif // WIN32
		
#ifdef WIN32
		E_API String(const wChar* str, Charset charset = Charset::UTF16, UINT32 size = MAX_UINT32);
#endif // WIN32
		
		E_API String(const String& str);

		E_API String(INT64 value, BaseSystem base = BaseSystem::DECIMAL);
		E_API String(UINT64 value, BaseSystem base = BaseSystem::DECIMAL);
		
		virtual ~String();

		E_API UINT32 length() const;
		E_API bool empty() const;
		E_API Charset charset() const;

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
		E_API INT32 toINT32(BaseSystem base = BaseSystem::DECIMAL) const;
		E_API UINT32 toUINT32(BaseSystem base = BaseSystem::DECIMAL) const;

		E_API INT64 toINT64(BaseSystem base = BaseSystem::DECIMAL) const;
		E_API UINT64 toUINT64(BaseSystem base = BaseSystem::DECIMAL) const;

		E_API void MatchEncode(const String& matchTo);
		E_API nCharArray&& toGBK() const;
		E_API nCharArray&& toUTF8() const;
		E_API wCharArray&& toUTF16() const;

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
}

#endif