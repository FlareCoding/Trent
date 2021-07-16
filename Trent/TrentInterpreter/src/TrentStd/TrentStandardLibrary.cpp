#include "TrentStandardLibrary.h"
#include <iostream>

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
}
