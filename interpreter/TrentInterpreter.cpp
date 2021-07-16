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

		// Initialize the global program-wide variable stack
		VariableStack_t program_stack;
		d_variable_stacks.push_back(program_stack);
	}

	TrentInterpreter::~TrentInterpreter()
	{
		TrentRuntime::Shutdown();
	}

	void TrentInterpreter::RegisterFunction(const std::string& name, TrentObject::member_fn_t fn)
	{
		d_registered_functions[name] = fn;
	}

	void TrentInterpreter::Interpret(const std::string& source)
	{
		parser::TrentParser parser;
		parser.Initialize(source);
		d_ast = parser.ConstructAST();

		for (auto& node : d_ast->d_root->d_children)
		{
			InterpretNode(node);
		}
	}

	void TrentInterpreter::InterpretNode(NodeRef<ASTNode> node)
	{
		switch (node->d_type)
		{
		case ASTNodeType::VariableDeclaration: {
			EvaluateVariableDeclarationNode(
				std::reinterpret_pointer_cast<ASTVariableDeclarationNode>(node)
			);
			break;
		}
		case ASTNodeType::FunctionCall: {
			TrentObject* result = EvaluateFunctionCallNode(
				std::reinterpret_pointer_cast<ASTFunctionCallNode>(node)
			);
			break;
		}
		case ASTNodeType::Assignment: {
			EvaluateAssignmentNode(
				std::reinterpret_pointer_cast<ASTAssignmentNode>(node)
			);
			break;
		}
		case ASTNodeType::Expression: {
			EvaluateExpressionNode(
				std::reinterpret_pointer_cast<ASTExpressionNode>(node)
			);
			break;
		}
		default: {
			break;
		}
		}
	}

	void TrentInterpreter::ExceptionObserver(TrentException* e)
	{
		printf("Interpreter encountered a TrentException (Line %zi)\n", d_current_lineno);
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

	TrentInterpreter::VariableStack_t& TrentInterpreter::GetCurrentVariableStack()
	{
		return d_variable_stacks[d_variable_stacks.size() - 1];
	}

	void TrentInterpreter::PushVariableStack()
	{
		VariableStack_t new_stack;
		d_variable_stacks.push_back(new_stack);
	}

	void TrentInterpreter::PopVariableStack()
	{
		if (d_variable_stacks.size() > 1)
			d_variable_stacks.pop_back();
	}

	TrentObject* TrentInterpreter::GetRegisteredVariable(const std::string& name)
	{
		// Iterating over variable stacks backwards
		for (std::vector<VariableStack_t>::reverse_iterator it = d_variable_stacks.rbegin(); 
			it != d_variable_stacks.rend(); ++it)
		{
			auto& stack = *it;
			if (stack.find(name) != stack.end())
				return stack[name];
		}

		return nullptr;
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

		TrentObject* value = EvaluateExpressionNode(node->d_value);
		
		auto& variable_stack = GetCurrentVariableStack();
		
		// Throw an exception if variable already exists,
		// otherwise register the new variable.
		if (variable_stack.find(node->d_variable_name) != variable_stack.end())
		{
			std::string ex_message = "Variable '" + node->d_variable_name + "' already exists";
			auto exception = TrentException("RuntimeException", ex_message, "VariableDeclarationError");
			exception.Raise();
			return TrentObject_Null;
		}

		variable_stack[node->d_variable_name] = value;
		return value;
	}

	TrentObject* TrentInterpreter::EvaluateVariableNode(NodeRef<ASTVariableNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

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

		// See if the variable already exists or not, if not, throw an exception.
		TrentObject* variable = GetRegisteredVariable(node->d_left->d_variable_name);
		if (!variable)
		{
			std::string ex_message = std::string("Variable '") + node->d_left->d_variable_name + "' does not exist";
			auto exception = TrentException("RuntimeException", ex_message, "EvaluateAssignmentNode");
			exception.Raise();
			return TrentObject_Null;
		}

		TrentObject* value = EvaluateExpressionNode(node->d_right);

		auto& variable_stack = GetCurrentVariableStack();
		variable_stack[node->d_left->d_variable_name] = value;

		return value;
	}

	TrentObject* TrentInterpreter::EvaluateFunctionCallNode(NodeRef<ASTFunctionCallNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		auto function = GetRegisteredFunction(node->d_function_name);

		if (!function &&
			d_ast->d_functions.find(node->d_function_name) == d_ast->d_functions.end())
		{
			std::string ex_message = "No function '" + node->d_function_name + "' exists";
			auto exception = TrentException("RuntimeException", ex_message, "FunctionCallError");
			exception.Raise();
			return TrentObject_Null;
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
			PushVariableStack();

			for (size_t i = 0; i < args.size(); i++)
			{
				// Add variables to the stack
				std::string variable_name = FunctionDeclarationNode->d_parameters[i]->d_variable_name;
				TrentObject* arg = args[i];

				RegisterVariable(variable_name, arg);
			}

			TrentObject* result = EvaluateUserDefinedFunctionCallNode(node->d_function_name);
			PopVariableStack();

			return result;
		}

		return function(MAKE_TRENT_TUPLE(args));
	}

	TrentObject* TrentInterpreter::EvaluateUserDefinedFunctionCallNode(const std::string& name)
	{
		auto FunctionDeclarationNode = d_ast->d_functions[name];

		for (auto& node : FunctionDeclarationNode->d_body)
		{
			InterpretNode(node);
		}

		return EvaluateExpressionNode(FunctionDeclarationNode->d_return_value);
	}

	TrentObject* TrentInterpreter::EvaluateExpressionNode(NodeRef<ASTExpressionNode> node)
	{
		// Tracking the current line of code.
		d_current_lineno = node->d_lineno;

		switch (node->d_value->d_type)
		{
		case ASTNodeType::Expression: {
			return EvaluateExpressionNode(
				std::reinterpret_pointer_cast<ASTExpressionNode>(node->d_value)
			);
		}
		case ASTNodeType::FunctionCall: {
			return EvaluateFunctionCallNode(
				std::reinterpret_pointer_cast<ASTFunctionCallNode>(node->d_value)
			);
		}
		case ASTNodeType::LiteralValue: {
			return EvaluateLiteralValueNode(
				std::reinterpret_pointer_cast<ASTLiteralValueNode>(node->d_value)
			);
		}
		case ASTNodeType::Variable: {
			return EvaluateVariableNode(
				std::reinterpret_pointer_cast<ASTVariableNode>(node->d_value)
			);
		}
		case ASTNodeType::BinaryOperator: {
			return EvaluateBinaryOperatorNode(
				std::reinterpret_pointer_cast<ASTBinaryOperatorNode>(node->d_value)
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
		case BinaryOperationType::ADD: {
			return left_val->__operator_add(right_val);
		}
		case BinaryOperationType::SUB: {
			return left_val->__operator_sub(right_val);
		}
		case BinaryOperationType::MUL: {
			return left_val->__operator_mul(right_val);
		}
		case BinaryOperationType::DIV: {
			return left_val->__operator_div(right_val);
		}
		default: {
			break;
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
			break;
		}
		case LiteralType::String: {
			return MAKE_TRENT_STRING(node->d_value.c_str());
			break;
		}
		default: {
			break;
		}
		}

		return TrentObject_Null;
	}
}