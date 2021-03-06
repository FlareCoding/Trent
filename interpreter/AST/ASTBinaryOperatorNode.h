#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTBinaryOperatorNode : public ASTNode
	{
	public:
		ASTBinaryOperatorNode();

		Operator d_op_type;
		NodeRef<ASTExpressionNode> d_left;
		NodeRef<ASTExpressionNode> d_right;
	};
}
