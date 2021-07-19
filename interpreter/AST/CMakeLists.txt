cmake_minimum_required(VERSION 3.0)

set(CMAKE_CURRENT_SOURCE_DIR interpreter/AST)

set(
    TRENTAST_HEADERS

    interpreter/AST/AST.h
    interpreter/AST/ASTNode.h
    interpreter/AST/ASTAssignmentNode.h
    interpreter/AST/ASTBinaryOperatorNode.h
    interpreter/AST/ASTBooleanOperatorNode.h
    interpreter/AST/ASTExpressionNode.h
    interpreter/AST/ASTFunctionCallNode.h
    interpreter/AST/ASTFunctionDeclarationNode.h
    interpreter/AST/ASTLiteralValueNode.h
    interpreter/AST/ASTProgramNode.h
    interpreter/AST/ASTVariableDeclarationNode.h
    interpreter/AST/ASTVariableNode.h
    interpreter/AST/ASTWhileLoopNode.h
    interpreter/AST/ASTForLoopNode.h
    interpreter/AST/ASTReturnStatementNode.h
    interpreter/AST/ASTBreakStatementNode.h
    interpreter/AST/ASTIfElseStatementNode.h
    interpreter/AST/ASTArrayValueNode.h

    PARENT_SCOPE
)

set(
    TRENTAST_SOURCES

    interpreter/AST/ASTNode.cpp
    interpreter/AST/ASTAssignmentNode.cpp
    interpreter/AST/ASTBinaryOperatorNode.cpp
    interpreter/AST/ASTBooleanOperatorNode.cpp
    interpreter/AST/ASTExpressionNode.cpp
    interpreter/AST/ASTFunctionCallNode.cpp
    interpreter/AST/ASTFunctionDeclarationNode.cpp
    interpreter/AST/ASTLiteralValueNode.cpp
    interpreter/AST/ASTProgramNode.cpp
    interpreter/AST/ASTVariableDeclarationNode.cpp
    interpreter/AST/ASTVariableNode.cpp
    interpreter/AST/ASTWhileLoopNode.cpp
    interpreter/AST/ASTForLoopNode.cpp
    interpreter/AST/ASTReturnStatementNode.cpp
    interpreter/AST/ASTBreakStatementNode.cpp
    interpreter/AST/ASTIfElseStatementNode.cpp
    interpreter/AST/ASTArrayValueNode.cpp

    PARENT_SCOPE
)
