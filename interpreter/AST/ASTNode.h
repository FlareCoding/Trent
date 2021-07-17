#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <cinttypes>

namespace trent
{
	enum class ASTNodeType
	{
		Empty,
		Program,
		LiteralValue,
		Expression,
		FunctionCall,
		FunctionDeclaration,
		Variable,
		VariableDeclaration,
		Block,
		BinaryOperator,
		BooleanOperator,
		Assignment,
		WhileLoop,
		ForLoop,
		ReturnStatement,
		BreakStatement,
		IfElseStatement
	};

	class ASTNode
	{
	public:
		ASTNodeType d_type		= ASTNodeType::Empty;
		size_t		d_lineno	= 0;
	};

	template <typename T>
	using NodeRef = std::shared_ptr<T>;

	template <typename T, typename... Args>
	constexpr NodeRef<T> MakeNode(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
