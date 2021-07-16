#pragma once
#include "../TrentInterpreter.h"

namespace trent
{
	class TrentStandardLibrary
	{
	public:
		static void Initialize(TrentInterpreter* env);

	private:
		static TrentObject* __TrentStdTypeof(TrentObject* args);
		static TrentObject* __TrentStdInstanceof(TrentObject* args);
		static TrentObject* __TrentStdPrint(TrentObject* args);
		static TrentObject* __TrentStdPrintln(TrentObject* args);
		static TrentObject* __TrentStdScan(TrentObject* args);
	};
}
