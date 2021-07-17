#pragma once
#include "ASTExpressionNode.h"

namespace trent
{
	using ConditionBodyBinding_t = std::pair<NodeRef<ASTExpressionNode>, std::vector<NodeRef<ASTNode>>>;

	class ASTIfElseStatementNode : public ASTNode
	{
	public:
		ASTIfElseStatementNode();

		ConditionBodyBinding_t				d_if_statement = { nullptr, {} };

		std::vector<ConditionBodyBinding_t> d_else_if_statements;
		std::vector<NodeRef<ASTNode>>		d_else_statement_body;
	};
}
