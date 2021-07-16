#include "ASTVariableNode.h"

namespace trent
{
	ASTVariableNode::ASTVariableNode(const std::string& var_name)
		: d_variable_name(var_name)
	{
		this->d_type = ASTNodeType::Variable;
	}
}
