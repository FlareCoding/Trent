#pragma once
#include "ASTNode.h"

namespace trent
{
	class ASTExpressionNode : public ASTNode
	{
	public:
		ASTExpressionNode();

		NodeRef<ASTNode> d_value;
	};
}
