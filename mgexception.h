#ifndef MGEXCEPTION_H
#define MGEXCEPTION_H

#include <string>
#include <exception>

namespace mg
{
    class Exception : public std::exception
    {
    public:
        Exception();
        Exception(std::string text);

        std::string text;
    };
} // end of mg namespace
#endif // MGEXCEPTION_H
