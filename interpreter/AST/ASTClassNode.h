#pragma once
#include "ASTFunctionDeclarationNode.h"
#include "ASTVariableDeclarationNode.h"

namespace trent
{
    class ASTClassNode : public ASTNode
	{
	public:
		ASTClassNode(const std::string& name = "");

		std::unordered_map<
			std::string,
			NodeRef<ASTFunctionDeclarationNode>
		> d_static_functions;

		std::unordered_map<
			std::string,
			NodeRef<ASTFunctionDeclarationNode>
		> d_member_functions;

		std::unordered_map<
			std::string,
			NodeRef<ASTVariableDeclarationNode>
		> d_member_vars;

		NodeRef<ASTFunctionDeclarationNode> d_constructor;
		std::string d_class_name;
	};
}
