#ifdef _WIN32
	#include <windows.h>
#elif __APPLE__
	#include "TargetConditionals.h"
#elif __linux__
	// linux
#elif __unix__ // all unices not caught above
	// Unix
#else
	#error "Unknown compiler"
#endif