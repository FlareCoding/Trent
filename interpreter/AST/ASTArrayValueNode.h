#pragma once
#include "ASTLiteralValueNode.h"
#include <vector>

namespace trent
{
	class ASTArrayValueNode : public ASTNode
	{
	public:
		ASTArrayValueNode();

		std::vector<NodeRef<ASTNode>> d_values;
	};
}
