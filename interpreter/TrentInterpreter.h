#pragma once
#include "Trent.h"
#include "Parser/TrentParser.h"

namespace trent
{
	using VariableStack_t = std::unordered_map<std::string, TrentObject*>;

	struct FunctionFrame
	{
		std::string name = "Anonymous";
		bool		return_state = false;

		std::vector<VariableStack_t> d_variable_stacks;
	};

	class TrentInterpreter
	{
	public:
		TrentInterpreter() = default;
		~TrentInterpreter();

		void InitializeEnvironment();

		void RegisterFunction(const std::string& name, TrentObject::member_fn_t fn);
		void Interpret(const std::string& source);

		void ExceptionObserver(TrentException* e);

	private:
		TrentObject* InterpretNode(NodeRef<ASTNode> node);

	private:
		std::unordered_map<std::string, TrentObject::member_fn_t> d_registered_functions;
		TrentObject::member_fn_t GetRegisteredFunction(const std::string& name);

		VariableStack_t& GetCurrentVariableStack();
		void PushVariableStack();
		void PopVariableStack();

		void RegisterVariable(const std::string& name, TrentObject* obj);
		TrentObject* GetRegisteredVariable(const std::string& name);
		void UpdateRegisteredVariable(const std::string& name, TrentObject* obj);

	private:
		// For tracking which function needs to return
		std::vector<FunctionFrame> d_function_call_stack;
		void PushFunctionFrame(const std::string& name);
		void PopFunctionFrame();
		bool DoesFunctionNeedReturning();
		void SetFunctionReturnState();
		FunctionFrame& GetCurrentFunctionFrame();

		// For tracking which loop needs to be broken out of
		std::vector<bool> d_loop_break_stack;
		void PushLoopFrame();
		void PopLoopFrame();
		bool DoesLoopNeedBreaking();
		void SetLoopBreakState();

	private:
		TrentObject* EvaluateLiteralValueNode(NodeRef<ASTLiteralValueNode> node);
		TrentObject* EvaluateExpressionNode(NodeRef<ASTExpressionNode> node);
		TrentObject* EvaluateBinaryOperatorNode(NodeRef<ASTBinaryOperatorNode> node);
		TrentObject* EvaluateBooleanOperatorNode(NodeRef<ASTBooleanOperatorNode> node);

		TrentObject* EvaluateWhileLoopNode(NodeRef<ASTWhileLoopNode> node);
		TrentObject* EvaluateForLoopNode(NodeRef<ASTForLoopNode> node);

		TrentObject* EvaluateReturnStatementNode(NodeRef<ASTReturnStatementNode> node);
		TrentObject* EvaluateBreakStatementNode(NodeRef<ASTBreakStatementNode> node);

		TrentObject* EvaluateVariableDeclarationNode(NodeRef<ASTVariableDeclarationNode> node);
		TrentObject* EvaluateVariableNode(NodeRef<ASTVariableNode> node);
		TrentObject* EvaluateAssignmentNode(NodeRef<ASTAssignmentNode> node);

		TrentObject* EvaluateFunctionCallNode(NodeRef<ASTFunctionCallNode> node);
		TrentObject* EvaluateUserDefinedFunctionCallNode(const std::string& name);


	private:
		std::shared_ptr<AST>	d_ast;
		size_t					d_current_lineno = 0;
	};
}
