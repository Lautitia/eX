#ifndef _EX_H_
#define _EX_H_

#include <memory>

#ifdef _WIN32
	#ifdef E_EXPORTS
		#define E_API __declspec(dllexport)
	#else
		#ifndef E_STATIC
			#define E_API __declspec(dllimport)
		#endif
	#endif
#endif

#ifndef E_API
	#define E_API
#endif


//Debug Level
#define ELOGERR		0
#define ELOGWARN	1
#define ELOGINFO	2
#define ELOGDDBG	3

#include "interf/types.h"
#include "interf/i_eX.h"

#endif