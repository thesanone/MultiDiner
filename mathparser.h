#ifndef MAHPARSER_H
#define MAHPARSER_H

#include "string"
#include <sstream>
#include <iostream>

using namespace std;

class MathParser
{
private:
    double number();
    double expr();
    double factor();
    double skobki();
    stringstream m_expr_stream;

public:
    double Calc(string str_expr)
    {
        m_expr_stream = stringstream(str_expr);
        return expr();
    }
};

inline double MathParser::number()
{
    int res = 0;
    for (;;)
    {
        char c = m_expr_stream.get();
        if (c >= '0' && c <= '9')
            res = res * 10 + c - '0';
        else
        {
            m_expr_stream.putback(c);
            return res;
        }
    }
}

inline double MathParser::skobki()
{
    char c = m_expr_stream.get();
    if (c == '(')
    {
        double x = expr();
        m_expr_stream.get();
        return x;
    }
    else
    {
        m_expr_stream.putback(c);
        return number();
    }
}

inline double MathParser::factor()
{
    double x = skobki();
    for (;;)
    {
        char c = m_expr_stream.get();
        switch (c)
        {
        case '*':
            x *= skobki();
            break;
        case '/':
            x /= skobki();
            break;
        default:
            m_expr_stream.putback(c);
            return x;
        }
    }
}

inline double MathParser::expr()
{
    double x = factor();
    for (;;)
    {
        char c = m_expr_stream.get();
        switch (c)
        {
        case '+':
            x += factor();
            break;
        case '-':
            x -= factor();
            break;
        default:
            m_expr_stream.putback(c);
            return x;
        }
    }
}

#endif // MAHPARSER_H
