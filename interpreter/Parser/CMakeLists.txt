cmake_minimum_required(VERSION 3.0)

set(CMAKE_CURRENT_SOURCE_DIR interpreter/Parser)

set(
    TRENTPARSER_HEADERS

    interpreter/Parser/Token.h
    interpreter/Parser/IdentifierToken.h
    interpreter/Parser/KeywordToken.h
    interpreter/Parser/LiteralValueToken.h
    interpreter/Parser/OperatorToken.h
    interpreter/Parser/SymbolToken.h
    interpreter/Parser/TrentLexer.h
    interpreter/Parser/TokenPool.h
    interpreter/Parser/TrentParser.h

    PARENT_SCOPE
)

set(
    TRENTPARSER_SOURCES

    interpreter/Parser/Token.cpp
    interpreter/Parser/IdentifierToken.cpp
    interpreter/Parser/KeywordToken.cpp
    interpreter/Parser/LiteralValueToken.cpp
    interpreter/Parser/OperatorToken.cpp
    interpreter/Parser/SymbolToken.cpp
    interpreter/Parser/TrentLexer.cpp
    interpreter/Parser/TokenPool.cpp
    interpreter/Parser/TrentParser.cpp

    PARENT_SCOPE
)
