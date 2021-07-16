#include "ASTLiteralValueNode.h"

namespace trent
{
	ASTLiteralValueNode::ASTLiteralValueNode(LiteralType type, const std::string& value)
		: d_value_type(type), d_value(value)
	{
		this->d_type = ASTNodeType::LiteralValue;
	}
}
