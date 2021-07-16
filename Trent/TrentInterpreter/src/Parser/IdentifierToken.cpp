#include "IdentifierToken.h"

namespace trent::parser
{
    IdentifierToken::IdentifierToken(const std::string& value)
        : d_value(value)
    {
        d_type = TokenType::Identifier;
    }
    
    std::string IdentifierToken::ToString()
    {
        return "[TokenType: Identifier, Value: '" + d_value + "']";
    }
}
