#include "mgexception.h"

#include <string>
#include <sstream>

using namespace mg;

Exception::Exception():exception()
{
    text = "";
    line = 0;
    function = "";
    timestamp = "";
}

Exception::Exception(std::string text, int line, std::string function, std::string timestamp):
  exception(),
  text(text),
  line(line),
  function(function),
  timestamp(timestamp)
{

}

const char *Exception::what() const throw()
{
  std::ostringstream oss;
  oss  << "Exception \""
       << text
       << "\" in function \""
       << function
       << "\" in line: "
       << line
       << ". Time: "
       << timestamp;
  return oss.str().c_str();
}
