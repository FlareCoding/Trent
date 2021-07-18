#include "TrentStandardLibrary.h"
#include <iostream>
#include <thread>

namespace trent
{
	char* __strremove(char* str, const char* sub)
	{
		size_t len = strlen(sub);
		if (len > 0) {
			char* p = str;
			while ((p = strstr(p, sub)) != NULL) {
				memmove(p, p + len, strlen(p + len) + 1);
			}
		}
		return str;
	}

	void TrentStandardLibrary::Initialize(TrentInterpreter* env)
	{
		env->RegisterFunction("typeof",		__TrentStdTypeof);
		env->RegisterFunction("instanceof", __TrentStdInstanceof);
		env->RegisterFunction("print",		__TrentStdPrint);
		env->RegisterFunction("println",	__TrentStdPrintln);
		env->RegisterFunction("scan",		__TrentStdScan);
		env->RegisterFunction("sleep",		__TrentStdSleep);
	}

	TrentObject* TrentStandardLibrary::__TrentStdTypeof(TrentObject* args)
	{
		TrentObject* obj;
		if (!TrentArg_Parse(args, "o", &obj))
		{
			auto exception = TrentException("TrentStandardLibrary", "Failed to parse arguments", "__TrentStdTypeof::Error");
			exception.Raise();
			return TrentObject_Null;
		}

		return MAKE_TRENT_STRING(obj->GetRuntimeName());
	}

	TrentObject* TrentStandardLibrary::__TrentStdInstanceof(TrentObject* args)
	{
		TrentObject* obj;
		if (!TrentArg_Parse(args, "o", &obj))
		{
			auto exception = TrentException("TrentStandardLibrary", "Failed to parse arguments", "__TrentStdInstanceof::Error");
			exception.Raise();
			return TrentObject_Null;
		}

		return MAKE_TRENT_STRING(obj->GetInstanceDescription());
	}

	TrentObject* TrentStandardLibrary::__TrentStdPrint(TrentObject* args)
	{
		TrentObject* obj = nullptr;
		char* ending = nullptr;
		if (!TrentArg_Parse(args, "os", &obj, &ending))
		{
			auto exception = TrentException("TrentStandardLibrary", "Failed to parse arguments", "__TrentStdPrint::Error");
			exception.Raise();
			return TrentObject_Null;
		}
		
		if (obj)
		{
			char* out = (char*)obj->ToString();
			__strremove(out, "'");
			printf("%s", out);
		}
		else
			printf("");

		if (ending)
			printf("%s", ending);

		return TrentObject_Null;
	}

	TrentObject* TrentStandardLibrary::__TrentStdPrintln(TrentObject* args)
	{
		TrentObject* obj = nullptr;
		if (!TrentArg_Parse(args, "o", &obj))
		{
			auto exception = TrentException("TrentStandardLibrary", "Failed to parse arguments", "__TrentStdPrintln::Error");
			exception.Raise();
			return TrentObject_Null;
		}

		if (obj)
		{
			char* out = (char*)obj->ToString();
			__strremove(out, "'");
			printf("%s\n", out);
		}
		else
			printf("\n");

		return TrentObject_Null;
	}
	
	TrentObject* TrentStandardLibrary::__TrentStdScan(TrentObject* args)
	{
		std::string input;
		std::getline(std::cin, input);

		return MAKE_TRENT_STRING(input.c_str());
	}

	TrentObject* TrentStandardLibrary::__TrentStdSleep(TrentObject* args)
	{
		int time = -1;
		char* time_unit = (char*)"ms";
		if (!TrentArg_Parse(args, "is", &time, &time_unit))
		{
			auto exception = TrentException("TrentStandardLibrary", "Failed to parse arguments", "__TrentStdSleep::Error");
			exception.Raise();
			return TrentObject_Null;
		}

		if (time <= 0)
			return TrentObject_Null;

		// time is in seconds
		if (strcmp(time_unit, "s") == 0)
			time *= 1000;
		else if (strcmp(time_unit, "us") == 0)
			time /= 1000;

		std::this_thread::sleep_for(std::chrono::milliseconds(time));
		return TrentObject_Null;
	}
}
