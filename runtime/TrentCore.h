#pragma once
#ifdef _WIN32
	#ifdef TR_BUILD
		#define TRAPI __declspec(dllexport)
	#else
		#define TRAPI __declspec(dllimport)
	#endif
#else
	#define TRAPI 
#endif