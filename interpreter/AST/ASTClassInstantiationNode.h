#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	class ASTClassInstantiationNode : public ASTNode
	{
	public:
		ASTClassInstantiationNode();

		std::string d_class_name;
		std::vector<NodeRef<ASTExpressionNode>> d_arguments;
	};
}
