#pragma once
#include <memory>
#include <vector>
#include "TrentObject.h"
#include "TrentException.h"

namespace trent
{
	struct StackFrame
	{
		std::vector<TrentObject*> d_objects;
		void Free();
	};

	class TrentRuntime
	{
	public:
		TRAPI static void Initialize();
		TRAPI static void Shutdown();

		template <class C, typename... Args>
		static TrentObject* AllocateObject(Args&&... args);

		TRAPI static void FreeObject(TrentObject* obj);

		TRAPI static void ElevateObjectToPreviousStack(TrentObject* obj);

		TRAPI static void PushStackFrame();
		TRAPI static void PopStackFrame();

	public:
		using exception_observer_fn_t = std::function<void(TrentException*)>;

		TRAPI static void RaiseException(TrentException* e);
		TRAPI static void SubscribeExceptionObserver(exception_observer_fn_t fn);

	private:
		TRAPI static std::vector<exception_observer_fn_t> d_exception_observers;

	private:
		TRAPI static std::vector<StackFrame> d_stack_frames;
		TRAPI static StackFrame& GetCurrentStackFrame();

		TRAPI static char* __strremove(char* str, const char* sub);
	};

	template<class C, typename... Args>
	inline TrentObject* TrentRuntime::AllocateObject(Args&&... args)
	{
		C* object = new C(std::forward<Args>(args)...);
		object->d_type.name = typeid(*object).name();
		__strremove((char*)object->d_type.name, "class trent::");

		object->d_type.size = sizeof(C);
		object->__Init();

		GetCurrentStackFrame().d_objects.push_back(object);
		return object;
	}
}
