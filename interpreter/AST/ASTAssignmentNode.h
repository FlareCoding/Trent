#pragma once
#include "ASTVariableNode.h"
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTAssignmentNode : public ASTNode
	{
	public:
		ASTAssignmentNode();

		NodeRef<ASTVariableNode>	d_left;
		NodeRef<ASTExpressionNode>	d_right;
	};
}
