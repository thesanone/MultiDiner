#ifndef MGEXCEPTION_H
#define MGEXCEPTION_H

#include <string>
#include <exception>

namespace mg
{
#define THROW_MG_EXCEPTION(s) throw Exception(s, __LINE__, __FUNCTION__, __TIMESTAMP__)
#define THROW_MG_NULL_POINTER_EXCEPTION(s) throw NullPointerException(s, __LINE__, __FUNCTION__, __TIMESTAMP__)
#define THROW_MG_VERTEX_EXISTING_EXCEPTION(s, vert, value, V, E) throw VertexExistingException<V, E>(s, __LINE__, __FUNCTION__, __TIMESTAMP__, vert, value)
#define THROW_MG_EDGE_EXISTING_EXCEPTION(s, edge, V, E) throw EdgeExistingException<V, E>(s, __LINE__, __FUNCTION__, __TIMESTAMP__, edge)
#define THROW_MG_ALLOCATOR_EXCEPTION(s) throw AllocatorException(s, __LINE__, __FUNCTION__, __TIMESTAMP__)

  enum ExceptionType { MG_EXCEPTION,
                     MG_NULL_POINTER_EXCEPTION,
                     MG_VERTEX_EXISTING_EXCEPTION,
                     MG_EDGE_EXISTING_EXCEPTION,
                     MG_ALLOCATOR_EXCEPTION };

  class Exception : public std::exception
  {
  public:
    Exception();
    Exception(std::string text, int line, std::string function, std::string timestamp);

    virtual const char* what() const throw();

    ExceptionType getExceptionType() const { return type; }

  protected:
    ExceptionType type;
    std::string text;
    int line;
    std::string function;
    std::string timestamp;
    mutable std::string fullString;
  };

  class NullPointerException: public Exception
  {
  public:
    NullPointerException(std::string text, int line, std::string function, std::string timestamp):
      Exception(text, line, function, timestamp)
    {
      type = MG_NULL_POINTER_EXCEPTION;
    }
  };


  template <typename V, typename E> class Edge;

  template<typename V, typename E> class Vertex;

  template<typename V, typename E>
  class VertexExistingException : public Exception
  {
  public:
    VertexExistingException(std::string text, int line, std::string function,
                            std::string timestamp, Vertex<V, E> *vertexP = NULL, V data = V()) :
      Exception(text, line, function, timestamp),
      vertexPointer(vertexP), data(data)
    {
      type = MG_VERTEX_EXISTING_EXCEPTION;
    }
    Vertex<V, E> *vertexPointer;
    V data;
  };

  template<typename V, typename E>
  class EdgeExistingException : public Exception
  {
  public:
    EdgeExistingException(std::string text, int line, std::string function,
                          std::string timestamp, Edge<V, E> *edgeP = NULL) :
      Exception(text, line, function, timestamp),
      edgePointer(edgeP)
    {
      type = MG_VERTEX_EXISTING_EXCEPTION;
    }

    Edge<V, E> *edgePointer;
  };

  class AllocatorException : public Exception
  {
  public:
    AllocatorException(std::string text, int line, std::string function, std::string timestamp):
      Exception(text, line, function, timestamp)
    {
      type = MG_ALLOCATOR_EXCEPTION;
    }
  };
} // end of mg namespace
#endif // MGEXCEPTION_H
