#ifndef MGEXCEPTION_H
#define MGEXCEPTION_H

#include <string>
#include <exception>

namespace mg
{
#define THROW_MG_EXCEPTION(s) throw Exception(s, __LINE__, __FUNCTION__, __TIMESTAMP__)

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
