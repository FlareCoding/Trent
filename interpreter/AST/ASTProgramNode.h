#pragma once
#include "ASTNode.h"

namespace trent
{
	class ASTProgramNode : public ASTNode
	{
	public:
		ASTProgramNode();

		std::vector<NodeRef<ASTNode>> d_children;
	};
}
