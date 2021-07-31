#include "ASTClassNode.h"

namespace trent
{
    ASTClassNode::ASTClassNode(const std::string& name)
        : d_class_name(name)
    {
        this->d_type = ASTNodeType::ClassDeclaration;
    }
}
