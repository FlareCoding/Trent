#include "LiteralValueToken.h"

namespace trent::parser
{
    LiteralValueToken::LiteralValueToken(LiteralType value_type, const std::string& value)
        : d_value_type(value_type), d_value(value)
    {
        d_type = TokenType::LiteralValue;
    }

    std::string LiteralValueToken::ToString()
    {
        return "[TokenType: LiteralValue, Value: '" + d_value + "' ValueType: '" + d_value_string_map[d_value_type] + "']";
    }
}
