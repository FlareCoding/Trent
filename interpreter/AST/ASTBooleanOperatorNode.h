#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTBooleanOperatorNode : public ASTNode
	{
	public:
		ASTBooleanOperatorNode();

		Operator d_op_type;
		NodeRef<ASTExpressionNode> d_left;
		NodeRef<ASTExpressionNode> d_right;
	};
}
