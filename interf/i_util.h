#ifndef _I_UTIL_H_
#define _I_UTIL_H_

namespace ex{
	template<typename T>
	char* BaseSystemConvert(T value, BaseSystem Base, char* str)
	{
		//
		if ( Base < BaseSystem::BINARY || Base > BaseSystem::HEX ){
			return nullptr;
		}
		if (value < 0){
			value = -value;
			*str++ = '-';
		}

		int mod = value % (int)Base;
		char* strPos = str;
		if ( mod != value ){
			strPos = BaseSystemConvert<T>(value/(int)Base, Base, str);
		}

		static const char NumberIndex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		*strPos++ = NumberIndex[mod];
		return strPos;
	}
}

#endif