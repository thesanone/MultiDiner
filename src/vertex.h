#ifndef VERTEX_H
#define VERTEX_H

#include "mgexception.h"
#include "edge.h"
#include <vector>
#include <algorithm>

namespace mg
{

template<typename V, typename E>
class Vertex
{
public:
  Vertex(V& dt);
  virtual ~Vertex();

  V getData() const;
  void setData(const V &value);

  std::vector<Edge<V, E>* >* getIncomingEdges();
  std::vector<Edge<V, E>* >* getOutgoingEdges();

  void addIncomingEdge(Edge<V, E> *edge);
  void addOutgoingEdge(Edge<V, E> *edge);

private:
  V data;
  std::vector<Edge<V, E>* > incomingEdges;
  std::vector<Edge<V, E>* > outgoingEdges;
};


// ********************************************************************************************
// *********************************** impelementatin *****************************************
// ********************************************************************************************


template<typename V, typename E> inline
Vertex<V, E>::Vertex(V& dt)
{
  data = dt;
}

template<typename V, typename E> inline
Vertex<V, E>::~Vertex()
{
  std::for_each(outgoingEdges.begin(), outgoingEdges.end(), [](Edge<V, E>* i) {delete (*i);});
  outgoingEdges.clear();
  incomingEdges.clear();
}

V Vertex<V, E>::getData() const
{
  return data;
}

void Vertex<V, E>::setData(const V &value)
{
  data = value;
}

template<typename V, typename E> inline
std::vector<Edge<V, E> *> *Vertex<V, E>::getIncomingEdges()
{
  return &incomingEdges;
}

template<typename V, typename E> inline
std::vector<Edge<V, E> *> *Vertex<V, E>::getOutgoingEdges()
{
  return &outgoingEdges;
}

template<typename V, typename E> inline
void Vertex<V, E>::addIncomingEdge(Edge<V, E> *edge)
{
  if(!edge)
  {
    throw Exception("Link to NULL!", __LINE__, __FUNCTION__, __TIMESTAMP__);
    return;
  }
  incomingEdges.push_back(edge);
}

template<typename V, typename E> inline
void Vertex<V, E>::addOutgoingEdge(Edge<V, E> *edge)
{
  if(!edge)
  {
    throw Exception("Link to NULL!", __LINE__, __FUNCTION__, __TIMESTAMP__);
    return;
  }
  outgoingEdges.push_back(edge);
}

} // end of mg namespace

#endif // VERTEX_H