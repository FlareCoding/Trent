#pragma once
#include "ASTVariableNode.h"
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTFunctionDeclarationNode :public ASTNode
	{
	public:
		ASTFunctionDeclarationNode();

		std::string								d_function_name;

		std::vector<NodeRef<ASTVariableNode>>	d_parameters;
		std::vector<NodeRef<ASTNode>>			d_body;

		// **Important: Will be set at runtime by the interpreter.
		NodeRef<ASTExpressionNode>				d_return_value;
	};
}
