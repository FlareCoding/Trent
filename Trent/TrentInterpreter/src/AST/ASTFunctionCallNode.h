#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTFunctionCallNode : public ASTNode
	{
	public:
		ASTFunctionCallNode();

		std::string d_function_name;
		std::vector<NodeRef<ASTExpressionNode>> d_arguments;
	};
}
