#include "TrentInterpreter.h"
#include "TrentStd/TrentStandardLibrary.h"

namespace trent
{
	void TrentInterpreter::InitializeEnvironment()
	{
		TrentRuntime::Initialize();
		TrentStandardLibrary::Initialize(this);

		TrentRuntime::SubscribeExceptionObserver([this](TrentException* e) {
			this->ExceptionObserver(e);
		});

		// Initialize the global program-wide frame with a variable stack
		PushFunctionFrame("$trent_$global_$main$");
	}

	TrentInterpreter::~TrentInterpreter()
	{
		TrentRuntime::PopStackFrame(); // popping the main program-wide function frame
		TrentRuntime::Shutdown();
	}

	void TrentInterpreter::RegisterFunction(const std::string& name, TrentObject::member_fn_t fn)
	{
		d_registered_functions[name] = fn;
	}

	void TrentInterpreter::Interpret(std::string& source)
	{
		parser::TrentParser parser;
		parser.Initialize(source);

		try {
			d_ast = parser.ConstructAST();
		}
		catch (...) {
			auto exception = TrentException("Parser", "Failed to construct AST tree, please check for syntax errors", "InternalError");
			exception.Raise();
			return;
		}


		for (auto& node : d_ast->d_root->d_children)
		{
			InterpretNode(node);
		}
	}

	TrentObject* TrentInterpreter::InterpretNode(NodeRef<ASTNode> node)
	{
		switch (node->d_type)
		{
		case ASTNodeType::VariableDeclaration: {
			EvaluateVariableDeclarationNode(
				std::static_pointer_cast<ASTVariableDeclarationNode>(node)
			);
			break;
		}
		case ASTNodeType::FunctionCall: {
			return EvaluateFunctionCallNode(
				std::static_pointer_cast<ASTFunctionCallNode>(node)
			);
		}
		case ASTNodeType::Assignment: {
			EvaluateAssignmentNode(
				std::static_pointer_cast<ASTAssignmentNode>(node)
			);
			break;
		}
		case ASTNodeType::Expression: {
			EvaluateExpressionNode(
				std::static_pointer_cast<ASTExpressionNode>(node)
			);
			break;
		}
		case ASTNodeType::WhileLoop: {
			return EvaluateWhileLoopNode(
				std::static_pointer_cast<ASTWhileLoopNode>(node)
			);
		}
		case ASTNodeType::ForLoop: {
			return EvaluateForLoopNode(
				std::static_pointer_cast<ASTForLoopNode>(node)
			);
		}
		case ASTNodeType::ReturnStatement: {
			return EvaluateReturnStatementNode(
				std::static_pointer_cast<ASTReturnStatementNode>(node)
			);
		}
		case ASTNodeType::BreakStatement: {
			EvaluateBreakStatementNode(
				std::static_pointer_cast<ASTBreakStatementNode>(node)
			);
			break;
		}
		case ASTNodeType::IfElseStatement: {
			return EvaluateIfElseStatementNode(
				std::static_pointer_cast<ASTIfElseStatementNode>(node)
			);
		}
		default: {
			break;
		}
		}

		return TrentObject_Null;
	}

	void TrentInterpreter::ExceptionObserver(TrentException* e)
	{
		auto& current_frame = GetCurrentFunctionFrame();

		printf("Interpreter encountered a TrentException:\n\tFunction: %s\n\t(Line %zi)\n", current_frame.name.c_str(), d_current_lineno);
		printf("%s\n", e->ToString().c_str());
		TrentRuntime::Shutdown();
		exit(1);
	}

	TrentObject::member_fn_t TrentInterpreter::GetRegisteredFunction(const std::string& name)
	{
		if (d_registered_functions.find(name) != d_registered_functions.end())
			return d_registered_functions[name];
		else
			return nullptr;
	}

	void TrentInterpreter::PushFunctionFrame(const std::string& name)
	{
		FunctionFrame frame;
		frame.name = name;
		frame.return_state = false;

		d_function_call_stack.push_back(frame);

		// Create the default frame-wide variable stack
		PushVariableStack();
	}

	void TrentInterpreter::PopFunctionFrame()
	{
		// The global program-wide frame should never be popped
		if (d_function_call_stack.size() > 1)
		{
			PopVariableStack();
			d_function_call_stack.pop_back();
		}
	}

	bool TrentInterpreter::DoesFunctionNeedReturning()
	{
		if (d_function_call_stack.size() < 2)
			return false;

		return d_function_call_stack[d_function_call_stack.size() - 1].return_state;
	}

	void TrentInterpreter::SetFunctionReturnState()
	{
		if (d_function_call_stack.size() > 1)
			d_function_call_stack[d_function_call_stack.size() - 1].return_state = true;
	}

	FunctionFrame& TrentInterpreter::GetCurrentFunctionFrame()
	{
		return d_function_call_stack[d_function_call_stack.size() - 1];
	}

	void TrentInterpreter::PushLoopFrame()
	{
		d_loop_break_stack.push_back({ false });
	}

	void TrentInterpreter::PopLoopFrame()
	{
		if (d_loop_break_stack.size())
			d_loop_break_stack.pop_back();
	}

	bool TrentInterpreter::DoesLoopNeedBreaking()
	{
		if (d_loop_break_stack.size() == 0)
			return false;

		return d_loop_break_stack[d_loop_break_stack.size() - 1];
	}

	void TrentInterpreter::SetLoopBreakState()
	{
		if (d_loop_break_stack.size())
			d_loop_break_stack[d_loop_break_stack.size() - 1] = true;
	}

	VariableStack_t& TrentInterpreter::GetCurrentVariableStack()
	{
		auto& current_frame = GetCurrentFunctionFrame();

		return current_frame.d_variable_stacks[current_frame.d_variable_stacks.size() - 1];
	}

	void TrentInterpreter::PushVariableStack()
	{
		TrentRuntime::PushStackFrame();

		auto& current_frame = GetCurrentFunctionFrame();

		VariableStack_t new_stack;
		current_frame.d_variable_stacks.push_back(new_stack);
	}

	void TrentInterpreter::PopVariableStack()
	{
		auto& current_frame = GetCurrentFunctionFrame();

		if (current_frame.d_variable_stacks.size() > 0)
		{
			current_frame.d_variable_stacks.pop_back();
			TrentRuntime::PopStackFrame();
		}
	}

	TrentObject* TrentInterpreter::GetRegisteredVariable(const std::string& name)
	{

		auto& current_frame = GetCurrentFunctionFrame();
		auto& global_frame = d_function_call_stack[0];

		// Checking for instance membership
		if (name == "this")
		{
			if (!current_frame.this_object)
				return TrentObject_Null;

			return current_frame.this_object;
		}

		// Iterating over variable stacks backwards in the current frame.
		for (std::vector<VariableStack_t>::reverse_iterator var_stack_it = current_frame.d_variable_stacks.rbegin();
			var_stack_it != current_frame.d_variable_stacks.rend(); ++var_stack_it)
		{
			auto& stack = *var_stack_it;
			if (stack.find(name) != stack.end())
				return stack[name];
		}

		// Iterating over variable stacks backwards in the global program frame.
		for (std::vector<VariableStack_t>::reverse_iterator var_stack_it = global_frame.d_variable_stacks.rbegin();
			var_stack_it != global_frame.d_variable_stacks.rend(); ++var_stack_it)
		{
			auto& stack = *var_stack_it;
			if (stack.find(name) != stack.end())
				return stack[name];
		}

		return nullptr;
	}

	void TrentInterpreter::UpdateRegisteredVariable(const std::string& name, TrentObject* obj)
	{
		auto& current_frame = GetCurrentFunctionFrame();
		auto& global_frame = d_function_call_stack[0];

		// Iterating over variable stacks backwards in the current frame.
		for (std::vector<VariableStack_t>::reverse_iterator var_stack_it = current_frame.d_variable_stacks.rbegin();
			var_stack_it != current_frame.d_variable_stacks.rend(); ++var_stack_it)
		{
			auto& stack = *var_stack_it;
			if (stack.find(name) != stack.end())
			{
				// No need to do anything if it's a self assignment
				if (stack[name] == obj)
					return;

				stack[name]->CopyFrom(obj);

				if (obj) {
					TrentRuntime::FreeObject(obj);
				}
				return;
			}
		}

		// Iterating over variable stacks backwards in the global program frame.
		for (std::vector<VariableStack_t>::reverse_iterator var_stack_it = global_frame.d_variable_stacks.rbegin();
			var_stack_it != global_frame.d_variable_stacks.rend(); ++var_stack_it)
		{
			auto& stack = *var_stack_it;
			if (stack.find(name) != stack.end())
			{
				stack[name]->CopyFrom(obj);

				if (obj) {
					TrentRuntime::FreeObject(obj);
				}
				return;
			}
		}
	}

	void TrentInterpreter::RegisterVariable(const std::string& name, TrentObject* obj)
	{
		auto& variable_stack = GetCurrentVariableStack();
		variable_stack[name] = obj;
	}

	TrentObject* TrentInterpreter::EvaluateVariableDeclarationNode(NodeRef<ASTVariableDeclarationNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		if (!GetRegisteredVariable(node->d_variable_name))
		{
			TrentObject* value = EvaluateExpressionNode(node->d_value);
			RegisterVariable(node->d_variable_name, value);
			return value;
		}
		else
		{
			std::string ex_message = "Variable '" + node->d_variable_name + "' already exists";
			auto exception = TrentException("RuntimeException", ex_message, "RegisterVariable");
			exception.Raise();
		}

		return nullptr;
	}

	TrentObject* TrentInterpreter::EvaluateVariableNode(NodeRef<ASTVariableNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		// Checking for member variables
		if (!node->d_parent_object.empty())
		{
			// Find variable object
			TrentObject* parent_object = GetRegisteredVariable(node->d_parent_object);
			if (!parent_object)
			{
				std::string ex_message = std::string("Variable '") + node->d_parent_object + "' does not exist";
				auto exception = TrentException("RuntimeException", ex_message, "EvaluateVariable");
				exception.Raise();
				return TrentObject_Null;
			}

			TrentObject* variable = parent_object->GetMemberVariable(node->d_variable_name.c_str());
			if (!variable)
			{
				std::string ex_message = std::string("Member variable '") + node->d_variable_name + "' does not exist";
				auto exception = TrentException("RuntimeException", ex_message, "EvaluateVariable");
				exception.Raise();
				return TrentObject_Null;
			}

			return variable;
		}

		// Checking static variables
		TrentObject* variable = GetRegisteredVariable(node->d_variable_name);
		if (!variable)
		{
			std::string ex_message = std::string("Variable '") + node->d_variable_name + "' does not exist";
			auto exception = TrentException("RuntimeException", ex_message, "EvaluateVariable");
			exception.Raise();
			return TrentObject_Null;
		}

		return variable;
	}

	TrentObject* TrentInterpreter::EvaluateAssignmentNode(NodeRef<ASTAssignmentNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		TrentObject* variable = nullptr;
		TrentObject* parent_object = nullptr;

		if (!node->d_parent_object.empty())
		{
			// Find parent object
			parent_object = GetRegisteredVariable(node->d_parent_object);
			if (!parent_object)
			{
				std::string ex_message = std::string("Variable '") + node->d_parent_object + "' does not exist";
				auto exception = TrentException("RuntimeException", ex_message, "EvaluateVariable");
				exception.Raise();
				return TrentObject_Null;
			}

			TrentObject* variable = parent_object->GetMemberVariable(node->d_left->d_variable_name.c_str());
			if (!variable)
			{
				std::string ex_message = std::string("Member variable '") + node->d_left->d_variable_name + "' does not exist";
				auto exception = TrentException("RuntimeException", ex_message, "EvaluateVariable");
				exception.Raise();
				return TrentObject_Null;
			}
		}
		else
		{
			// See if the variable already exists or not, if not, throw an exception.
			TrentObject* variable = GetRegisteredVariable(node->d_left->d_variable_name);
			if (!variable)
			{
				std::string ex_message = std::string("Variable '") + node->d_left->d_variable_name + "' does not exist";
				auto exception = TrentException("RuntimeException", ex_message, "EvaluateAssignmentNode");
				exception.Raise();
				return TrentObject_Null;
			}
		}

		TrentObject* value = EvaluateExpressionNode(node->d_right);

		if (!parent_object)
			UpdateRegisteredVariable(node->d_left->d_variable_name, value);
		else
			parent_object->SetMemberVariable(node->d_left->d_variable_name.c_str(), value);

		return value;
	}

	TrentObject* TrentInterpreter::EvaluateFunctionCallNode(NodeRef<ASTFunctionCallNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		TrentObject::MemberFunctionData function_data = { nullptr };
		TrentObject* parent_object = nullptr;

		// Check if function is a global static function or a class member function
		if (!node->d_parent_object.empty())
		{
			// Attempt to find static class function
			if (d_ast->d_classes.find(node->d_parent_object) != d_ast->d_classes.end())
			{
				auto class_node = d_ast->d_classes[node->d_parent_object];

				if (class_node->d_static_functions.find(node->d_function_name) != class_node->d_static_functions.end())
				{
					return EvaluateClassStaticFunctionCallNode(node, class_node);
				}
			}

			// Find parent object
			parent_object = GetRegisteredVariable(node->d_parent_object);
			if (!parent_object)
			{
				std::string ex_message = std::string("Variable '") + node->d_parent_object + "' does not exist";
				auto exception = TrentException("RuntimeException", ex_message, "EvaluateVariable");
				exception.Raise();
				return TrentObject_Null;
			}

			function_data = parent_object->GetMemberFunction(node->d_function_name.c_str());

			if (!(function_data.d_func_decl || function_data.d_compiled_fn) &&
				d_ast->d_functions.find(node->d_function_name) == d_ast->d_functions.end())
			{
				std::string ex_message = std::string(parent_object->GetRuntimeName()) + " has no member function '" + node->d_function_name + "'";
				auto exception = TrentException("RuntimeException", ex_message, "FunctionCallError");
				exception.Raise();
				return TrentObject_Null;
			}
		}
		else
		{
			function_data.d_compiled_fn = GetRegisteredFunction(node->d_function_name);

			if (!function_data.d_compiled_fn &&
				d_ast->d_functions.find(node->d_function_name) == d_ast->d_functions.end())
			{
				std::string ex_message = "No function '" + node->d_function_name + "' exists";
				auto exception = TrentException("RuntimeException", ex_message, "FunctionCallError");
				exception.Raise();
				return TrentObject_Null;
			}
		}

		// Getting function arguments
		std::vector<TrentObject*> args;
		for (auto& arg_node : node->d_arguments)
		{
			TrentObject* t_arg = EvaluateExpressionNode(arg_node);
			if (!t_arg)
			{
				auto exception = TrentException("RuntimeException", "Failed to evaluate argument expression", "FunctionCallError");
				exception.Raise();
				continue;
			}

			args.push_back(t_arg);
		}

		// Function is a user defined function
		if (d_ast->d_functions.find(node->d_function_name) != d_ast->d_functions.end())
		{
			auto FunctionDeclarationNode = d_ast->d_functions[node->d_function_name];

			// Register arguments as new variables on the newly created stack
			PushFunctionFrame(node->d_function_name);

			for (size_t i = 0; i < std::min(args.size(), FunctionDeclarationNode->d_parameters.size()); i++)
			{
				// Add variables to the stack
				std::string variable_name = FunctionDeclarationNode->d_parameters[i]->d_variable_name;
				TrentObject* arg = args[i];

				RegisterVariable(variable_name, arg);
			}

			TrentObject* result = EvaluateUserDefinedFunctionCallNode(FunctionDeclarationNode);
			TrentRuntime::ElevateObjectToPreviousStack(result);

			PopFunctionFrame();

			return result;
		}

		if (function_data.d_compiled_fn)
			return function_data.d_compiled_fn(MAKE_TRENT_TUPLE(args));

		// If it's not a compiled function, then it must
		// be a user-defined class member function.
		if (!function_data.d_func_decl)
		{
			auto exception = TrentException("RuntimeException", "Member function is nullptr", "FunctionCallError");
			exception.Raise();
			return TrentObject_Null;
		}

		// Register arguments as new variables on the newly created stack
		PushFunctionFrame(node->d_function_name);

		if (parent_object)
		{
			// Set the stack's "this" object
			auto& current_frame = GetCurrentFunctionFrame();
			current_frame.this_object = parent_object;
		}

		for (size_t i = 0; i < std::min(args.size(), function_data.d_func_decl->d_parameters.size()); i++)
		{
			// Add variables to the stack
			std::string variable_name = function_data.d_func_decl->d_parameters[i]->d_variable_name;
			TrentObject* arg = args[i];

			RegisterVariable(variable_name, arg);
		}

		TrentObject* result = EvaluateUserDefinedFunctionCallNode(function_data.d_func_decl);
		TrentRuntime::ElevateObjectToPreviousStack(result);

		PopFunctionFrame();

		return result;
	}

	TrentObject* TrentInterpreter::EvaluateUserDefinedFunctionCallNode(NodeRef<ASTFunctionDeclarationNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		for (auto& node : node->d_body)
		{
			TrentObject* interpreted_result = InterpretNode(node);

			// Check if current function needs returning
			if (DoesFunctionNeedReturning())
				return interpreted_result;
		}

		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateClassStaticFunctionCallNode(NodeRef<ASTFunctionCallNode> node, NodeRef<ASTClassNode> class_node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		// Getting function arguments
		std::vector<TrentObject*> args;
		for (auto& arg_node : node->d_arguments)
		{
			TrentObject* t_arg = EvaluateExpressionNode(arg_node);
			if (!t_arg)
			{
				auto exception = TrentException("RuntimeException", "Failed to evaluate argument expression", "FunctionCallError");
				exception.Raise();
				continue;
			}

			args.push_back(t_arg);
		}

		auto FunctionDeclarationNode = class_node->d_static_functions[node->d_function_name];

		// Register arguments as new variables on the newly created stack
		PushFunctionFrame(node->d_function_name);

		for (size_t i = 0; i < std::min(args.size(), FunctionDeclarationNode->d_parameters.size()); i++)
		{
			// Add variables to the stack
			std::string variable_name = FunctionDeclarationNode->d_parameters[i]->d_variable_name;
			TrentObject* arg = args[i];

			RegisterVariable(variable_name, arg);
		}

		TrentObject* result = EvaluateUserDefinedFunctionCallNode(FunctionDeclarationNode);
		TrentRuntime::ElevateObjectToPreviousStack(result);

		PopFunctionFrame();

		return result;
	}

	TrentObject* TrentInterpreter::EvaluateClassInstantiationNode(NodeRef<ASTClassInstantiationNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		auto class_name = node->d_class_name;
		if (d_ast->d_classes.find(class_name) == d_ast->d_classes.end())
		{
			auto exception = TrentException("RuntimeException", "Class definition for class '" + class_name + "' not found", "ClassInstantiationError");
			exception.Raise();
			return TrentObject_Null;
		}

		auto class_decl = d_ast->d_classes[class_name];

		// Getting constructor arguments
		std::vector<TrentObject*> constructor_arguments;
		for (auto& arg_node : node->d_arguments)
		{
			TrentObject* t_arg = EvaluateExpressionNode(arg_node);
			if (!t_arg)
			{
				auto exception = TrentException("RuntimeException", "Failed to evaluate argument expression", "EvaluateClassInstantiationNode");
				exception.Raise();
				continue;
			}

			constructor_arguments.push_back(t_arg);
		}

		auto instance = TrentRuntime::AllocateObject<TrentObject>(true);

		// Registering member functions
		for (auto& pair : class_decl->d_member_functions)
		{
			auto fn_name = pair.first;
			auto func_decl_node = pair.second;

			TrentObject::MemberFunctionData fn_data = { nullptr, nullptr };
			fn_data.d_func_decl = func_decl_node;

			instance->AddMemberFunction(fn_name.c_str(), fn_data);
		}

		// Registering member variables
		for (auto& pair : class_decl->d_member_vars)
		{
			auto var_name = pair.first;
			auto var_decl_node = pair.second;
			TrentObject* var_value = EvaluateExpressionNode(var_decl_node->d_value);

			instance->AddMemberVariable(var_name.c_str(), var_value);
		}

		// Finally, call the constructor if it exists
		if (class_decl->d_constructor)
		{
			// Register arguments as new variables on the newly created stack
			PushFunctionFrame(class_decl->d_class_name + ".constructor");

			// Set the stack's "this" object
			auto& current_frame = GetCurrentFunctionFrame();
			current_frame.this_object = instance;

			for (size_t i = 0; i < std::min(constructor_arguments.size(), class_decl->d_constructor->d_parameters.size()); i++)
			{
				// Add variables to the stack
				std::string variable_name = class_decl->d_constructor->d_parameters[i]->d_variable_name;
				TrentObject* arg = constructor_arguments[i];

				RegisterVariable(variable_name, arg);
			}

			TrentObject* result = EvaluateUserDefinedFunctionCallNode(class_decl->d_constructor);
			TrentRuntime::ElevateObjectToPreviousStack(result);

			PopFunctionFrame();
		}

		return instance;
	}

	TrentObject* TrentInterpreter::EvaluateExpressionNode(NodeRef<ASTExpressionNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		switch (node->d_value->d_type)
		{
		case ASTNodeType::Expression: {
			return EvaluateExpressionNode(
				std::static_pointer_cast<ASTExpressionNode>(node->d_value)
			);
		}
		case ASTNodeType::ClassInstantiation: {
			return EvaluateClassInstantiationNode(
				std::static_pointer_cast<ASTClassInstantiationNode>(node->d_value)
			);
		}
		case ASTNodeType::FunctionCall: {
			return EvaluateFunctionCallNode(
				std::static_pointer_cast<ASTFunctionCallNode>(node->d_value)
			);
		}
		case ASTNodeType::LiteralValue: {
			return EvaluateLiteralValueNode(
				std::static_pointer_cast<ASTLiteralValueNode>(node->d_value)
			);
		}
		case ASTNodeType::ArrayValue: {
			return EvaluateArrayValueNode(
				std::static_pointer_cast<ASTArrayValueNode>(node->d_value)
			);
		}
		case ASTNodeType::Variable: {
			return EvaluateVariableNode(
				std::static_pointer_cast<ASTVariableNode>(node->d_value)
			);
		}
		case ASTNodeType::BinaryOperator: {
			return EvaluateBinaryOperatorNode(
				std::static_pointer_cast<ASTBinaryOperatorNode>(node->d_value)
			);
		}
		case ASTNodeType::BooleanOperator: {
			return EvaluateBooleanOperatorNode(
				std::static_pointer_cast<ASTBooleanOperatorNode>(node->d_value)
			);
		}
		default: {
			break;
		}
		}

		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateBinaryOperatorNode(NodeRef<ASTBinaryOperatorNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		TrentObject* left_val = EvaluateExpressionNode(node->d_left);
		TrentObject* right_val = EvaluateExpressionNode(node->d_right);

		switch (node->d_op_type)
		{
		case Operator::Add: {
			return left_val->__operator_add(right_val);
		}
		case Operator::Sub: {
			return left_val->__operator_sub(right_val);
		}
		case Operator::Mul: {
			return left_val->__operator_mul(right_val);
		}
		case Operator::Div: {
			return left_val->__operator_div(right_val);
		}
		default: {
			break;
		}
		}

		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateBooleanOperatorNode(NodeRef<ASTBooleanOperatorNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		// The node isn't guaranteed to have a left expression,
		// for example (!x) expression only has the ! operator
		// and a right hand side x.
		TrentObject* left_val = nullptr;
		if (node->d_left)
			left_val = EvaluateExpressionNode(node->d_left);

		TrentObject* right_val = EvaluateExpressionNode(node->d_right);

		auto LeftValueMustExist = [&]() {
			if (left_val == nullptr)
			{
				auto exception = TrentException("RuntimeException", "No left hand side value provided", "EvaluateBooleanOperation");
				exception.Raise();
			}
		};

		auto ValueMustBeBoolean = [](TrentObject* value, const std::string& value_side) {
			if (strcmp(value->GetRuntimeName(), "Boolean") != 0)
			{
				auto exception = TrentException("RuntimeException", value_side + " hand side must be a boolean", "EvaluateBooleanOperation");
				exception.Raise();
			}
		};

		switch (node->d_op_type)
		{
		case Operator::LessThan: {
			LeftValueMustExist();
			return left_val->__operator_lt(right_val);
		}
		case Operator::GreaterThan: {
			LeftValueMustExist();
			return left_val->__operator_gt(right_val);
		}
		case Operator::LessThanOrEqual: {
			LeftValueMustExist();
			return left_val->__operator_ltoe(right_val);
		}
		case Operator::GreaterThanOrEqual: {
			LeftValueMustExist();
			return left_val->__operator_gtoe(right_val);
		}
		case Operator::Equequ: {
			LeftValueMustExist();
			return left_val->__operator_equequ(right_val);
		}
		case Operator::Notequ: {
			LeftValueMustExist();
			return left_val->__operator_notequ(right_val);
		}
		case Operator::Not: {
			ValueMustBeBoolean(right_val, "right");

			bool current_bool_value = reinterpret_cast<TrentBoolean*>(right_val)->GetValue();
			return MAKE_TRENT_BOOLEAN(!current_bool_value);
		}
		case Operator::And: {
			ValueMustBeBoolean(left_val, "left");
			ValueMustBeBoolean(right_val, "right");

			bool current_left_bool_value = reinterpret_cast<TrentBoolean*>(left_val)->GetValue();
			bool current_right_bool_value = reinterpret_cast<TrentBoolean*>(right_val)->GetValue();

			return MAKE_TRENT_BOOLEAN(current_left_bool_value && current_right_bool_value);
		}
		case Operator::Or: {
			ValueMustBeBoolean(left_val, "left");
			ValueMustBeBoolean(right_val, "right");

			bool current_left_bool_value = reinterpret_cast<TrentBoolean*>(left_val)->GetValue();
			bool current_right_bool_value = reinterpret_cast<TrentBoolean*>(right_val)->GetValue();

			return MAKE_TRENT_BOOLEAN(current_left_bool_value || current_right_bool_value);
		}
		default: {
			break;
		}
		}

		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateWhileLoopNode(NodeRef<ASTWhileLoopNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		// Register loop on the break stack
		PushLoopFrame();

		auto condition_object = EvaluateExpressionNode(node->d_condition);
		if (strcmp(condition_object->GetRuntimeName(), "Boolean") != 0)
		{
			auto exception = TrentException("RuntimeException", "condition expression must be a boolean", "EvaluateWhileLoopNode");
			exception.Raise();
			return TrentObject_Null;
		}

		while (reinterpret_cast<TrentBoolean*>(condition_object)->GetValue())
		{
			// Create new variable stack
			PushVariableStack();

			for (auto& body_node : node->d_body)
			{
				TrentObject* interpreted_result = InterpretNode(body_node);

				// Check if current function needs returning
				if (DoesFunctionNeedReturning())
				{
					// Free up the memory from the previous condition state object
					TrentRuntime::FreeObject(condition_object);

					// Elevate the returning result to the
					// previous stack frame to preserve its lifetime.
					TrentRuntime::ElevateObjectToPreviousStack(interpreted_result);

					// Pop the variable stack
					PopVariableStack();

					return interpreted_result;
				}

				// Check if loop needs to be broken out of
				if (DoesLoopNeedBreaking())
					break;
			}

			// Pop the variable stack
			PopVariableStack();

			// Check if loop needs to be broken out of
			// (needs to be done again to prevent condition loop
			// from being checked again (this local C++ code).
			if (DoesLoopNeedBreaking())
				break;

			// Free up the memory from the previous condition state object
			TrentRuntime::FreeObject(condition_object);

			// Re-evaluate the condition object
			condition_object = EvaluateExpressionNode(node->d_condition);
		}

		// Remove loop from the break stack
		PopLoopFrame();

		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateForLoopNode(NodeRef<ASTForLoopNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		// Register loop on the break stack
		PushLoopFrame();

		// Create a new variable stack for the initializer
		PushVariableStack();

		// Run the initializer
		InterpretNode(node->d_initializer);

		auto condition_object = EvaluateExpressionNode(node->d_condition);
		if (strcmp(condition_object->GetRuntimeName(), "Boolean") != 0)
		{
			auto exception = TrentException("RuntimeException", "condition expression must be a boolean", "EvaluateForLoopNode");
			exception.Raise();
			return TrentObject_Null;
		}

		while (reinterpret_cast<TrentBoolean*>(condition_object)->GetValue())
		{
			// Create new variable stack for the loop's body
			PushVariableStack();

			// Execute body statements
			for (auto& body_node : node->d_body)
			{
				TrentObject* interpreted_result = InterpretNode(body_node);

				// Check if current function needs returning
				if (DoesFunctionNeedReturning())
				{
					// Free up the memory from the previous condition state object
					TrentRuntime::FreeObject(condition_object);

					// Elevate the returning result to the
					// previous stack frame to preserve its lifetime.
					TrentRuntime::ElevateObjectToPreviousStack(interpreted_result);

					// Pop the variable stack
					PopVariableStack();

					return interpreted_result;
				}

				// Check if loop needs to be broken out of
				if (DoesLoopNeedBreaking())
					break;
			}

			// Pop the body's variable stack
			PopVariableStack();

			// Check if loop needs to be broken out of
			// (needs to be done again to prevent condition loop
			// from being checked again (this local C++ code).
			if (DoesLoopNeedBreaking())
				break;

			// Run the increment
			InterpretNode(node->d_increment);

			// Free up the memory from the previous condition state object
			TrentRuntime::FreeObject(condition_object);

			// Re-evaluate the condition object
			condition_object = EvaluateExpressionNode(node->d_condition);
		}

		// Pop the variable stack created for the initializer
		PopVariableStack();

		// Remove loop from the break stack
		PopLoopFrame();

		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateReturnStatementNode(NodeRef<ASTReturnStatementNode> node)
	{
		SetFunctionReturnState();
		return EvaluateExpressionNode(node->d_return_value);
	}

	TrentObject* TrentInterpreter::EvaluateBreakStatementNode(NodeRef<ASTBreakStatementNode> node)
	{
		SetLoopBreakState();
		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateIfElseStatementNode(NodeRef<ASTIfElseStatementNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		// Check the "if" condition
		auto condition_object = EvaluateExpressionNode(node->d_if_statement.first);
		if (strcmp(condition_object->GetRuntimeName(), "Boolean") != 0)
		{
			auto exception = TrentException("RuntimeException", "condition expression must be a boolean", "EvaluateIfElseStatementNode");
			exception.Raise();
			return TrentObject_Null;
		}

		if (reinterpret_cast<TrentBoolean*>(condition_object)->GetValue())
		{
			// Create new variable stack
			PushVariableStack();

			for (auto& body_node : node->d_if_statement.second)
			{
				TrentObject* interpreted_result = InterpretNode(body_node);

				// Check if current function needs returning
				if (DoesFunctionNeedReturning())
				{
					// Free up the memory from the previous condition state object
					TrentRuntime::FreeObject(condition_object);

					// Elevate the returning result to the
					// previous stack frame to preserve its lifetime.
					TrentRuntime::ElevateObjectToPreviousStack(interpreted_result);

					// Pop the variable stack
					PopVariableStack();

					return interpreted_result;
				}

				// Check if loop needs to be broken out of
				if (DoesLoopNeedBreaking())
					break;
			}

			// Free up the memory from the previous condition state object
			TrentRuntime::FreeObject(condition_object);

			// Pop the variable stack
			PopVariableStack();
		}
		else
		{
			bool need_to_execute_else_block = true;

			// Check if "elif" statements need to be executed
			if (node->d_else_if_statements.size())
			{
				for (auto& elif_statement : node->d_else_if_statements)
				{
					condition_object = EvaluateExpressionNode(elif_statement.first);
					if (strcmp(condition_object->GetRuntimeName(), "Boolean") != 0)
					{
						auto exception = TrentException("RuntimeException", "condition expression must be a boolean", "EvaluateIfElseStatementNode");
						exception.Raise();
						return TrentObject_Null;
					}

					if (reinterpret_cast<TrentBoolean*>(condition_object)->GetValue())
					{
						need_to_execute_else_block = false;

						// Create new variable stack
						PushVariableStack();

						for (auto& body_node : elif_statement.second)
						{
							TrentObject* interpreted_result = InterpretNode(body_node);

							// Check if current function needs returning
							if (DoesFunctionNeedReturning())
							{
								// Free up the memory from the previous condition state object
								TrentRuntime::FreeObject(condition_object);

								// Elevate the returning result to the
								// previous stack frame to preserve its lifetime.
								TrentRuntime::ElevateObjectToPreviousStack(interpreted_result);

								// Pop the variable stack
								PopVariableStack();

								return interpreted_result;
							}

							// Check if loop needs to be broken out of
							if (DoesLoopNeedBreaking())
								break;
						}

						// Pop the variable stack
						PopVariableStack();

						// Stop looping through elif statements
						break;
					}
				}
			}

			// Check if "else" block needs execution
			if (need_to_execute_else_block && node->d_else_statement_body.size())
			{
				// Create new variable stack
				PushVariableStack();

				for (auto& body_node : node->d_else_statement_body)
				{
					TrentObject* interpreted_result = InterpretNode(body_node);

					// Check if current function needs returning
					if (DoesFunctionNeedReturning())
					{
						// Free up the memory from the previous condition state object
						TrentRuntime::FreeObject(condition_object);

						// Elevate the returning result to the
						// previous stack frame to preserve its lifetime.
						TrentRuntime::ElevateObjectToPreviousStack(interpreted_result);

						// Pop the variable stack
						PopVariableStack();

						return interpreted_result;
					}

					// Check if loop needs to be broken out of
					if (DoesLoopNeedBreaking())
						break;
				}

				// Pop the variable stack
				PopVariableStack();
			}
		}

		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateLiteralValueNode(NodeRef<ASTLiteralValueNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		switch (node->d_value_type)
		{
		case LiteralType::Integer: {
			return MAKE_TRENT_INT(std::stoi(node->d_value));
		}
		case LiteralType::String: {
			return MAKE_TRENT_STRING(node->d_value.c_str());
		}
		case LiteralType::Boolean: {
			bool val = (node->d_value == "true");
			return MAKE_TRENT_BOOLEAN(val);
		}
		case LiteralType::Float: {
			return MAKE_TRENT_FLOAT(std::stof(node->d_value));
		}
		default: {
			break;
		}
		}

		return TrentObject_Null;
	}

	TrentObject* TrentInterpreter::EvaluateArrayValueNode(NodeRef<ASTArrayValueNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		std::vector<TrentObject*> array_items;

		for (auto& item_node : node->d_values)
		{
			if (item_node->d_type == ASTNodeType::LiteralValue)
			{
				// parse literal value
				auto item = EvaluateLiteralValueNode(
					std::static_pointer_cast<ASTLiteralValueNode>(item_node)
				);

				array_items.push_back(item);
			}
			else if (item_node->d_type == ASTNodeType::ArrayValue)
			{
				// parse child array value
				auto item = EvaluateArrayValueNode(
					std::static_pointer_cast<ASTArrayValueNode>(item_node)
				);

				array_items.push_back(item);
			}
			else if (item_node->d_type == ASTNodeType::Variable)
			{
				// parse child array value
				auto item = EvaluateVariableNode(
					std::static_pointer_cast<ASTVariableNode>(item_node)
				);

				array_items.push_back(item);
			}
		}

		return MAKE_TRENT_ARRAY(array_items);
	}
}
