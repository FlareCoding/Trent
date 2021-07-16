#pragma once
#include "ASTNode.h"

namespace trent
{
	class ASTVariableNode : public ASTNode
	{
	public:
		ASTVariableNode(const std::string& var_name = "");

		std::string d_variable_name;
	};

}
