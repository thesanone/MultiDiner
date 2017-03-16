#include "mgexception.h"

using namespace mg;

Exception::Exception():exception()
{
    text = "";
}

Exception::Exception(std::string text):exception()
{
    this->text = text;
}
