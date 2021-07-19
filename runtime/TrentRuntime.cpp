#include "TrentRuntime.h"

namespace trent
{
	std::vector<StackFrame> TrentRuntime::d_stack_frames;
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

	void StackFrame::Free()
	{
		for (std::vector<TrentObject*>::reverse_iterator it = d_objects.rbegin();
			it != d_objects.rend(); ++it)
		{
			TrentObject* obj = *it;
			delete obj;
		}
	}

	StackFrame& TrentRuntime::GetCurrentStackFrame()
	{
		static StackFrame s_empty;

		if (!d_stack_frames.size())
			return s_empty;

		return d_stack_frames[d_stack_frames.size() - 1];
	}

	void TrentRuntime::Initialize()
	{
		PushStackFrame();

		TrentObject_Null = AllocateObject<TrentObject>(true);
		TrentObject_Null->d_description = "null";
	}

	void TrentRuntime::Shutdown()
	{
		PopStackFrame();
	}

	void TrentRuntime::FreeObject(TrentObject* obj)
	{
		auto& current_frame = GetCurrentStackFrame();

		auto it = std::find(current_frame.d_objects.begin(), current_frame.d_objects.end(), obj);
		if (it != current_frame.d_objects.end())
		{
			current_frame.d_objects.erase(it);
			delete obj;
		}
	}

	void TrentRuntime::ElevateObjectToPreviousStack(TrentObject* obj)
	{
		if (d_stack_frames.size() < 2)
			return;

		auto& current_frame = GetCurrentStackFrame();

		auto it = std::find(current_frame.d_objects.begin(), current_frame.d_objects.end(), obj);
		if (it != current_frame.d_objects.end())
		{
			// Erase from current stack frame
			current_frame.d_objects.erase(it);

			// Add object to the previous stack frame
			auto& previous_frame_objects = d_stack_frames[d_stack_frames.size() - 2].d_objects;
			previous_frame_objects.insert(previous_frame_objects.begin(), obj);
		}
	}

	void TrentRuntime::PushStackFrame()
	{
		StackFrame frame;
		d_stack_frames.push_back(frame);
		//printf("Pushed stack frame...\n");
	}

	void TrentRuntime::PopStackFrame()
	{
		GetCurrentStackFrame().Free();
		d_stack_frames.pop_back();
		//printf("Popped stack frame...\n");
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
