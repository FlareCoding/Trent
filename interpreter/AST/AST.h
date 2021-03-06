#pragma once
#include "ASTProgramNode.h"
#include "ASTFunctionCallNode.h"
#include "ASTExpressionNode.h"
#include "ASTLiteralValueNode.h"
#include "ASTArrayValueNode.h"
#include "ASTVariableNode.h"
#include "ASTVariableDeclarationNode.h"
#include "ASTBinaryOperatorNode.h"
#include "ASTBooleanOperatorNode.h"
#include "ASTFunctionDeclarationNode.h"
#include "ASTAssignmentNode.h"
#include "ASTWhileLoopNode.h"
#include "ASTForLoopNode.h"
#include "ASTReturnStatementNode.h"
#include "ASTBreakStatementNode.h"
#include "ASTIfElseStatementNode.h"
#include "ASTClassNode.h"
#include "ASTClassInstantiationNode.h"

namespace trent
{
	class AST
	{
	public:
		NodeRef<ASTProgramNode>	d_root;

		std::unordered_map<
			std::string,
			NodeRef<ASTFunctionDeclarationNode>
		> d_functions;

		std::unordered_map<
			std::string,
			NodeRef<ASTClassNode>
		> d_classes;
	};
}
