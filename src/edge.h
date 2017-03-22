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
    /*this->source = src;
    this->destenation = dst;
    this->value = value;*/
  }

  Vertex<V, E>* getDestenation() const {return destenation;}
  //void setDestenation(const V &value) {destenation = value;}

  Vertex<V, E>* getSource() const {return source;}
  //void setSource(const V &value) {source = value;}

  E getValue() const {return value;}
  void setValue(const E &value) {this->value = value;}

private:
  Vertex<V, E>* destenation;
  Vertex<V, E>* source;
  E value;
};

} // end of namespace

#endif // EDGE_H
