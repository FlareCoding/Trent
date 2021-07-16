#pragma once
#include "ASTNode.h"

namespace trent
{
	enum class LiteralType
	{
		Null,
		Integer,
		String,
		Boolean
	};

	class ASTLiteralValueNode : public ASTNode
	{
	public:
		ASTLiteralValueNode(LiteralType type = LiteralType::Null, const std::string& value = "");

		LiteralType d_value_type = LiteralType::Null;
		std::string d_value;
	};
}
