#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTWhileLoopNode : public ASTNode
	{
	public:
		ASTWhileLoopNode();

		NodeRef<ASTExpressionNode> d_condition;
		std::vector<NodeRef<ASTNode>> d_body;
	};
}
