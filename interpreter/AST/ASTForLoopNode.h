#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTForLoopNode : public ASTNode
	{
	public:
		ASTForLoopNode();

		NodeRef<ASTNode>			d_initializer;
		NodeRef<ASTExpressionNode>	d_condition;
		NodeRef<ASTNode>			d_increment;

		std::vector<NodeRef<ASTNode>> d_body;
	};
}
