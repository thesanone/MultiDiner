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
  type(MG_EXCEPTION),
  text(text),
  line(line),
  function(function),
  timestamp(timestamp)
{

}

const char *Exception::what() const throw()
{
  std::ostringstream oss;
  switch(type)
  {
    case(MG_EXCEPTION):
      oss << "mg::Exception\n";
      break;

    case(MG_NULL_POINTER_EXCEPTION):
      oss << "mg::NullPointerException\n";
      break;

    case(MG_VERTEX_EXISTING_EXCEPTION):
      oss << "mg::VertexExistingException\n";
      break;

    case(MG_EDGE_EXISTING_EXCEPTION):
      oss << "mg::EdgeExistingException\n";
      break;

    case(MG_ALLOCATOR_EXCEPTION):
      oss << "mg::AllocatorException\n";
      break;
  }

  oss  << "\""
       << text
       << "\" in function \""
       << function
       << "\" in line: "
       << line
       << ". Build from: "
       << timestamp;
  fullString = oss.str();
  return fullString.c_str();
}
