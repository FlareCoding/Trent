#include "SymbolToken.h"

namespace trent::parser
{
    SymbolToken::SymbolToken(Symbol symbol, const std::string& value)
        : d_symbol(symbol), d_value(value)
    {
        d_type = TokenType::Symbol;
    }

    std::string SymbolToken::ToString()
    {
        return "[TokenType: Symbol, Value: '" + d_value + "']";
    }
}
