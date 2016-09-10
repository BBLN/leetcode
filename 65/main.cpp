#include <vector>
using namespace std;

class Solution {
public:
    enum class Symbol
    {
        Plus,
        Minus,
        Digit,
        Exponent,
        FloatingPoint    
    };
    
    bool buildAST(string & s, vector<Symbol> & AST)
    {
        auto it = s.begin();
        auto end = s.end();
        
        for (; it != end; ++it)
        {
            if (*it == ' ')
            {
                continue;
            }
            break;
        }
    
        for (; it != end; ++it)
        {
            auto c = *it;
            
            if (c >= '0' && c <= '9')
            {
                AST.emplace_back(Symbol::Digit);
                continue;
            }
            
            if (c == '-')
            {
                AST.emplace_back(Symbol::Minus);
                continue;
            }
            
            if (c == '+')
            {
                AST.emplace_back(Symbol::Plus);
                continue;
            }
            
            if (c == 'e')
            {
                AST.emplace_back(Symbol::Exponent);
                continue;
            }
            
            if (c == '.')
            {
                AST.emplace_back(Symbol::FloatingPoint);
                continue;
            }
            
            if (c == ' ')
            {
                break;
            }
            
            return false;
        }
        
        // After, we must have a space
        for (; it != end; ++it)
        {
            if (*it == ' ')
            {
                continue;
            }
            return false;
        }
        
        return true;
    }
    
    enum class State : uint32_t
    {
        Digits = 1,
        Exponent = 2,
        Floating = 4
    };
    
    bool inState(uint32_t states, std::initializer_list<State> needed)
    {
        for (auto state : needed)
        {
            if (states & static_cast<uint32_t>(state))
            {
                return true;
            }
        }
        
        return false;
    }
    
    void on(uint32_t & states, State state)
    {
        states |= static_cast<uint32_t>(state);
    }
    
    void off(uint32_t & states, State state)
    {
        states &= ~static_cast<uint32_t>(state);
    }
    
    bool isNumber(string s) {
        vector<Symbol> AST;
        if (false == buildAST(s, AST))
        {
            return false;
        }
       
        uint32_t parser = 0;
        
        for (auto & symbol : AST)
        {
            switch (symbol)
            {
                case Symbol::Plus:
                case Symbol::Minus:
                    // Must be before any floating or digits, might be after exponent
                    if (inState(parser, { State::Floating, State::Digits }))
                    {
                        return false;
                    }
                    break;
                    
                case Symbol::Exponent:
                    // Must have a value before exponent
                    if (!inState(parser, { State::Digits }))
                    {
                        return false;
                    }
                    
                    if (inState(parser, { State::Exponent }))
                    {
                        return false;
                    }
                    
                    // Exponent mode
                    on(parser, State::Exponent);
                    
                    // Not floating point anymore
                    off(parser, State::Floating);
                    
                    // No digits anymore
                    off(parser, State::Digits);
                    break;
                case Symbol::FloatingPoint:
                    // Exponent can't have a float point
                    // Float point should be once
                    if (inState(parser, { State::Floating, State::Exponent }))
                    {
                        return false;
                    }
                    on(parser, State::Floating);
                    break;
                case Symbol::Digit:
                    on(parser, State::Digits);
                    break;
                default:
                    return false;
            }
        }
        
        return inState(parser, { State::Digits });
    }
};

