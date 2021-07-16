#include "TrentRuntime.h"

namespace trent
{
	std::vector<TrentObject*> TrentRuntime::d_object_pool;
	std::vector<TrentRuntime::exception_observer_fn_t> TrentRuntime::d_exception_observers;

	char* TrentRuntime::__strremove(char* str, const char* sub)
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

	void TrentRuntime::Initialize()
	{
		TrentObject_Null = AllocateObject<TrentObject>();
		TrentObject_Null->d_description = "null";
	}

	void TrentRuntime::Shutdown()
	{
		for (auto& ptr : d_object_pool)
		{
			delete ptr;
		}

		d_object_pool.clear();
	}
	
	void TrentRuntime::RaiseException(TrentException* e)
	{
		for (auto& observer : d_exception_observers)
		{
			observer(e);
		}
	}
	
	void TrentRuntime::SubscribeExceptionObserver(exception_observer_fn_t fn)
	{
		d_exception_observers.push_back(fn);
	}
}
