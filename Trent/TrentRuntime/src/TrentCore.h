#pragma once
#ifdef TR_BUILD
#define TRAPI __declspec(dllexport)
#else
#define TRAPI __declspec(dllimport)
#endif