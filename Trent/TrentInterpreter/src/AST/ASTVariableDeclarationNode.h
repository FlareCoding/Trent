#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTVariableDeclarationNode : public ASTNode
	{
	public:
		ASTVariableDeclarationNode();

		std::string d_variable_name;
		NodeRef<ASTExpressionNode> d_value;
	};
}
