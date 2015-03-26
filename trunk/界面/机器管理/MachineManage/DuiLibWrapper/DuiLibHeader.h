
#include "..\DuiLib\UIlib.h"

#include "UICrack.h"

namespace DuiLib {
// tString is a TCHAR std::string
//typedef std::basic_string<TCHAR> tString;
}
#ifndef NO_USING_DUILIB_NAMESPACE
	using namespace DuiLib;
	using namespace std;
#endif


#include <olectl.h>

#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))


#ifdef Release_DuilibStatic

#else

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\bin\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\bin\\DuiLib_d.lib")
#		
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\bin\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\bin\\DuiLib.lib")
#   endif
#endif

#endif


	