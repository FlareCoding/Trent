#pragma once
#include "TrentException.h"
#include <cinttypes>
#include <ostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <cstdarg>

namespace trent
{
	struct ObjectType
	{
		const char* name;
		size_t		size;
	};

	class TrentObject
	{
		friend class TrentRuntime;

	public:
		using getter_fn_t = std::function<TrentObject*()>;
		using setter_fn_t = std::function<void(TrentObject* args)>;
		using member_fn_t = std::function<TrentObject*(TrentObject* args)>;

	public:
		TRAPI virtual ~TrentObject() = default;
		TRAPI virtual void __Init();
		TRAPI virtual const char* GetRuntimeName();
		TRAPI virtual const char* ToString();
		TRAPI const char* GetInstanceDescription();

		TRAPI virtual TrentObject* Copy(bool delegate_ownership_to_runtime = true);
		TRAPI virtual void CopyFrom(TrentObject* other);

		/*
		*	Binary Operators
		*/

		TRAPI virtual TrentObject* __operator_add(TrentObject* obj);
		TRAPI virtual TrentObject* __operator_sub(TrentObject* obj);
		TRAPI virtual TrentObject* __operator_mul(TrentObject* obj);
		TRAPI virtual TrentObject* __operator_div(TrentObject* obj);

		/*
		*	Boolean Operators
		*/

		// Less than boolean operator
		TRAPI virtual TrentObject* __operator_lt(TrentObject* obj);

		// Less than or equal to boolean operator
		TRAPI virtual TrentObject* __operator_gt(TrentObject* obj);
		
		// Greater than boolean operator
		TRAPI virtual TrentObject* __operator_ltoe(TrentObject* obj);

		// Greater than or equal to boolean operator
		TRAPI virtual TrentObject* __operator_gtoe(TrentObject* obj);

		// Equal to boolean operator
		TRAPI virtual TrentObject* __operator_equequ(TrentObject* obj);

		// Not equal to boolean operator
		TRAPI virtual TrentObject* __operator_notequ(TrentObject* obj);

		ObjectType d_type;

		TRAPI void AddGetter(const char* property, getter_fn_t fn);
		TRAPI void AddSetter(const char* property, setter_fn_t fn);
		TRAPI void AddMemberFunction(const char* fn_name, member_fn_t fn);

		TRAPI getter_fn_t GetPropertyGetter(const char* fn_name);
		TRAPI setter_fn_t GetPropertySetter(const char* fn_name);
		TRAPI member_fn_t GetMemberFunction(const char* fn_name);

		TRAPI TrentObject* Call(const char* fn_name, TrentObject* args = nullptr);

		TRAPI friend std::ostream& operator<<(std::ostream& os, TrentObject& obj);

	private:
		std::string d_description;
		std::string d_instance_description;

		std::unordered_map<std::string, getter_fn_t> d_property_getters;
		std::unordered_map<std::string, setter_fn_t> d_property_setters;
		std::unordered_map<std::string, member_fn_t> d_member_functions;
	};

	TRAPI extern TrentObject* TrentObject_Null;

	TRAPI bool TrentArg_Parse(TrentObject* obj, const char* fmt, ...);

#define TRENT_REGISTER_FUNCTION_LAMBDA(fn_name) \
				[this](TrentObject* args) -> TrentObject* { return fn_name(args); }

#define PARAM_LIST_IL(...)	std::initializer_list<TrentObject*>{__VA_ARGS__}
#define PARAM_LIST_VEC(...) std::vector<TrentObject*>{__VA_ARGS__}

#define MAKE_TRENT_INT(val)			TrentRuntime::AllocateObject<TrentInteger>(true, val)
#define MAKE_TRENT_STRING(val)		TrentRuntime::AllocateObject<TrentString>(true, val)
#define MAKE_TRENT_BOOLEAN(val)		TrentRuntime::AllocateObject<TrentBoolean>(true, val)
#define MAKE_TRENT_FLOAT(val)		TrentRuntime::AllocateObject<TrentFloat>(true, val)
#define MAKE_TRENT_TUPLE_IL(...)	TrentRuntime::AllocateObject<TrentTuple>(true, PARAM_LIST_IL(__VA_ARGS__))
#define MAKE_TRENT_TUPLE_VEC(...)	TrentRuntime::AllocateObject<TrentTuple>(true, PARAM_LIST_VEC(__VA_ARGS__))
#define MAKE_TRENT_TUPLE(val)		TrentRuntime::AllocateObject<TrentTuple>(true, val)
#define MAKE_TRENT_ARRAY_IL(...)	TrentRuntime::AllocateObject<TrentArray>(true, PARAM_LIST_IL(__VA_ARGS__))
#define MAKE_TRENT_ARRAY_VEC(...)	TrentRuntime::AllocateObject<TrentArray>(true, PARAM_LIST_VEC(__VA_ARGS__))
#define MAKE_TRENT_ARRAY(val)		TrentRuntime::AllocateObject<TrentArray>(true, val)

#define MAKE_TRENT_INT_SPEC_OWNERSHIP(runtime_ownership, val)		TrentRuntime::AllocateObject<TrentInteger>(runtime_ownership, val)
#define MAKE_TRENT_STRING_SPEC_OWNERSHIP(runtime_ownership, val)	TrentRuntime::AllocateObject<TrentString>(runtime_ownership, val)
#define MAKE_TRENT_BOOLEAN_SPEC_OWNERSHIP(runtime_ownership, val)	TrentRuntime::AllocateObject<TrentBoolean>(runtime_ownership, val)
#define MAKE_TRENT_FLOAT_SPEC_OWNERSHIP(runtime_ownership, val)		TrentRuntime::AllocateObject<TrentFloat>(runtime_ownership, val)
#define MAKE_TRENT_TUPLE_IL_SPEC_OWNERSHIP(runtime_ownership, ...)	TrentRuntime::AllocateObject<TrentTuple>(runtime_ownership, PARAM_LIST_IL(__VA_ARGS__))
#define MAKE_TRENT_TUPLE_VEC_SPEC_OWNERSHIP(runtime_ownership, ...)	TrentRuntime::AllocateObject<TrentTuple>(runtime_ownership, PARAM_LIST_VEC(__VA_ARGS__))
#define MAKE_TRENT_TUPLE_SPEC_OWNERSHIP(runtime_ownership, val)		TrentRuntime::AllocateObject<TrentTuple>(runtime_ownership, val)
#define MAKE_TRENT_ARRAY_IL_SPEC_OWNERSHIP(runtime_ownership, ...)	TrentRuntime::AllocateObject<TrentArray>(runtime_ownership, PARAM_LIST_IL(__VA_ARGS__))
#define MAKE_TRENT_ARRAY_VEC_SPEC_OWNERSHIP(runtime_ownership, ...)	TrentRuntime::AllocateObject<TrentArray>(runtime_ownership, PARAM_LIST_VEC(__VA_ARGS__))
#define MAKE_TRENT_ARRAY_SPEC_OWNERSHIP(runtime_ownership, val)		TrentRuntime::AllocateObject<TrentArray>(runtime_ownership, val)
}
