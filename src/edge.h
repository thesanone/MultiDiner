#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

namespace mg
{

template <typename V, typename E>
class Edge
{
public:
  Edge() {}
  Edge(Vertex<V, E>* src, Vertex<V, E>* dst, E value):destenation(dst), source(src), value(value)
  {

  }

  Vertex<V, E>* getDestination() const {return destenation;}

  Vertex<V, E>* getSource() const {return source;}

  E getValue() const {return value;}
  void setValue(const E &value) {this->value = value;}

private:
  Vertex<V, E>* destenation;
  Vertex<V, E>* source;
  E value;
};

} // end of namespace

#endif // EDGE_H
