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
    Exception(std::string text, int line, std::string function, std::string timestamp);

    virtual const char* what() const throw();


  private:
    std::string text;
    int line;
    std::string function;
    std::string timestamp;
  };
} // end of mg namespace
#endif // MGEXCEPTION_H
