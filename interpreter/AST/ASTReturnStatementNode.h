#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTReturnStatementNode : public ASTNode
	{
	public:
		ASTReturnStatementNode();

		NodeRef<ASTExpressionNode>	d_return_value;
	};
}
