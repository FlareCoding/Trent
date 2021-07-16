#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	enum class BinaryOperationType
	{
		ADD,
		SUB,
		MUL,
		DIV
	};

	class ASTBinaryOperatorNode : public ASTNode
	{
	public:
		ASTBinaryOperatorNode();

		BinaryOperationType d_op_type;
		NodeRef<ASTExpressionNode> d_left;
		NodeRef<ASTExpressionNode> d_right;
	};
}
