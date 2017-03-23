#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include "vertex.h"
#include "edge.h"
#include "mgexception.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

namespace mg
{
  template <typename V, typename E>
  class Multigraph
  {
  public:
    Multigraph() {}

    void addVertex(V value);
    void addEdge(V src, V dst, E value);

    void clear();

    void deleteVertex(V value);
    bool vertexIsIsolated(V value);

    void generateDotText(std::string name);

    std::vector<Vertex<V, E>> getVertexes() const;

    class VertexIterator
    {
    public:
      VertexIterator(const VertexIterator& iterator);
      VertexIterator(const size_t position, std::vector<Vertex<V, E>>* vertex);
      VertexIterator& operator ++ ();
      VertexIterator& operator += (const size_t k);
      VertexIterator& operator -= (const size_t k);
      bool operator == (const VertexIterator& iterator);
      bool operator != (const VertexIterator& iterator);
      Vertex<V, E>* operator * ();

    private:
      size_t _position;
      std::vector<Vertex<V, E>*>* _vertexP;
    };

    VertexIterator beginV() {return VertexIterator(0, &vertexes);}
    VertexIterator endV() {return VertexIterator(vertexes.size(), &vertexes);}

    template <typename V2, typename E2>
    friend std::ostream& operator<< (std::ostream& os, const Multigraph<V2, E2>& dt);

    template <typename V2, typename E2>
    friend std::istream& operator>> (std::istream& is, Multigraph<V2, E2>& dt);

  private:
    class Allocator
    {
    public:
      Allocator(){}
      virtual ~Allocator();

      Vertex<V, E>* getVertex(V& dt);
      Edge<V, E>* getEdge(Vertex<V, E>* src, Vertex<V, E>* dst, E value);

      void returnVertex(Vertex<V, E>* vertex);
      void returnEdge(Edge<V, E>* edge);

      void returnAll();

    private:
      std::vector<Vertex<V, E>*> vertexes_pool;
      std::vector<Edge<V, E>*> edges_pool;
    };
    Allocator alloc;

  protected:
    std::vector<Vertex<V, E>*> vertexes;
  };


  // ********************************************************************************************
  // *********************************** impelementatin *****************************************
  // ********************************************************************************************


  template <typename V, typename E>
  std::ostream& operator<< (std::ostream& os, const Multigraph<V, E>& dt)
  {
    /*os << dt.vertexes.size() << "\n";
    std::for_each(dt.vertexes.begin(), dt.vertexes.end(), [&os](V i){ os << i << "\n"; });

    os << dt.edges.size() << "\n";
    std::for_each(dt.edges.begin(), dt.edges.end(), [&os](Edge<V, E> i)
    {
      os << i.getSource() << "\n"
         << i.getDestenation() << "\n"
         << i.getValue() << "\n";
    });

    return os;*/

    os << dt.vertexes.size() << "\n";/*
    std::for_each (vertexes.begin(), vertexes.end(), [&outputFile](Vertex<V, E>* i)
    {
             outputFile <<"\"" << i->getData() <<"\"" << ";\n";
    });

    std::for_each (vertexes.begin(), vertexes.end(), [&outputFile](Vertex<V, E>* i)
    {
      auto outgoingEdges = i->getOutgoingEdges();

      std::for_each(outgoingEdges->begin(), outgoingEdges->end(), [&outputFile](Edge<V, E>* j)
      {
        outputFile <<"\"" << j->getSource()->getData() <<"\""
                   << "->"
                   <<"\"" << j->getDestenation()->getData() <<"\""
                   << "[label=\""
                   << j->getValue()
                   << "\"];\n";
      });

    });
    outputFile << "}";
    outputFile.close();*/
  }

  template <typename V, typename E>
  std::istream& operator>> ( std::istream& is, Multigraph<V, E>& dt )
  {
     size_t vertexesSize, edgesSize;

     is >> vertexesSize;

     V obj;
     for(size_t i = 0; i != vertexesSize; i++)
     {
       is >> obj;
       dt.vertexes.push_back(obj);
     }

     is >> edgesSize;
     V obj2;
     E valueObj;
     for(size_t i = 0; i != edgesSize; i++)
     {
       is >> obj
          >> obj2
          >> valueObj;
       dt.edges.push_back(Edge<V, E>(obj, obj2, valueObj));
     }

     return is;
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::addVertex(V value)
  {
    bool alredyExist = false;
    std::for_each(vertexes.begin(), vertexes.end(), [value, &alredyExist](Vertex<V, E>* i)
    {
      if(i->getData() == value)
        alredyExist = true;
    });
    if(alredyExist)
    {
      //throw Exception("Vertex already exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      THROW_MG_EXCEPTION("Vertex already exist!");
      return;
    }

    vertexes.push_back(alloc.getVertex(value));
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::addEdge(V src, V dst, E value)
  {
    if(src == dst)
    {
      //throw Exception("The multigraph prevents the creation of loops!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      THROW_MG_EXCEPTION("The multigraph prevents the creation of loops!");
      return;
    }

    Vertex<V, E>* srcPointer = NULL;
    Vertex<V, E>* dstPointer = NULL;
    std::for_each(vertexes.begin(), vertexes.end(), [src, dst, &srcPointer, &dstPointer](Vertex<V, E>* i)
    {
      auto cur = i->getData();
      if(cur == src)
        srcPointer = i;
      if(cur == dst)
        dstPointer = i;
    });

    if(!srcPointer)
    {
      //throw Exception("Src vertex doesn't exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      THROW_MG_EXCEPTION("Src vertex doesn't exist!");
      return;
    }

    if(!dstPointer)
    {
      throw Exception("Dst vertex doesn't exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      THROW_MG_EXCEPTION("Dst vertex doesn't exist!");
      return;
    }

    auto newEdge = alloc.getEdge(srcPointer, dstPointer, value);
    srcPointer->addOutgoingEdge(newEdge);
    dstPointer->addIncomingEdge(newEdge);
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::clear()
  {
    vertexes.clear();
    alloc.returnAll();
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::deleteVertex(V value)
  {
    Vertex<V, E>* vertexPointer = NULL;
    std::for_each(vertexes.begin(), vertexes.end(), [value, &vertexPointer](Vertex<V, E>* i)
    {
      if(i->getData() == value)
        vertexPointer = i;
    });

    if(!vertexPointer)
    {
      //throw Exception("Vertex doesn't exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      THROW_MG_EXCEPTION("Vertex doesn't exist!");
      return;
    }

    auto vertexIncomingEdges = vertexPointer->getIncomingEdges();
    auto vertexOutgoingEdges = vertexPointer->getOutgoingEdges();

    std::for_each(vertexIncomingEdges->begin(), vertexIncomingEdges->end(),
                  [this](Edge<V, E>* i)
    {
      i->getSource()->delOutgoingEdge(i);
      i->getDestenation()->delIncomingEdge(i);
      alloc.returnEdge(i);
    });

    std::for_each(vertexOutgoingEdges->begin(), vertexOutgoingEdges->end(),
                  [this](Edge<V, E>* i)
    {
      i->getDestenation()->delIncomingEdge(i);
      i->getSource()->delOutgoingEdge(i);
      alloc.returnEdge(i);
    });

    alloc.returnVertex(vertexPointer);
  }

  template<typename V, typename E> inline
  bool Multigraph<V, E>::vertexIsIsolated(V value)
  {
    Vertex<V, E>* vertexPointer = NULL;
    std::for_each(vertexes.begin(), vertexes.end(), [value, &vertexPointer](Vertex<V, E>* i)
    {
      if(i->getData() == value)
        vertexPointer = i;
    });

    if(!vertexPointer)
    {
      //throw Exception("Vertex doesn't exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      THROW_MG_EXCEPTION("Vertex doesn't exist!");
      return false;
    }

    auto vertexIncomingEdges = vertexPointer->getIncomingEdges();
    auto vertexOutgoingEdges = vertexPointer->getOutgoingEdges();

    return (vertexIncomingEdges && vertexIncomingEdges->empty() &&
            vertexOutgoingEdges && vertexOutgoingEdges->empty());
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::generateDotText(std::string name)
  {
    std::ofstream outputFile;
    outputFile.open(name);
    outputFile << "digraph {\n";
    std::for_each (vertexes.begin(), vertexes.end(), [&outputFile](Vertex<V, E>* i)
    {
             outputFile <<"\"" << i->getData() <<"\"" << ";\n";
    });

    std::for_each (vertexes.begin(), vertexes.end(), [&outputFile](Vertex<V, E>* i)
    {
      auto outgoingEdges = i->getOutgoingEdges();

      std::for_each(outgoingEdges->begin(), outgoingEdges->end(), [&outputFile](Edge<V, E>* j)
      {
        outputFile <<"\"" << j->getSource()->getData() <<"\""
                   << "->"
                   <<"\"" << j->getDestenation()->getData() <<"\""
                   << "[label=\""
                   << j->getValue()
                   << "\"];\n";
      });

    });
    outputFile << "}";
    outputFile.close();
  }

  template<typename V, typename E> inline
  std::vector<Vertex<V, E> > Multigraph<V, E>::getVertexes() const
  {
    return vertexes;
  }

  template<typename V, typename E> inline
  Multigraph<V, E>::VertexIterator::VertexIterator(const Multigraph<V, E>::VertexIterator &iterator)
  {
    _vertexP = iterator._vertexP;
    _position = iterator._position;
  }

  template<typename V, typename E> inline
  Multigraph<V, E>::VertexIterator::VertexIterator(const size_t position, std::vector<Vertex<V, E>> *vertex)
  {
    _position = position;
    _vertexP = vertex;
  }

  template<typename V, typename E>
  typename Multigraph<V, E>::VertexIterator &Multigraph<V, E>::VertexIterator::operator ++()
  {
    if(_vertexP->size() < (_position + 1))
    {
      throw Exception("Null pointer exception, icrement > MultiGraph-vertexes-size!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return *this;
    }
    _position++;
    return *this;
  }

  template<typename V, typename E> inline
  typename Multigraph<V, E>::VertexIterator &Multigraph<V, E>::VertexIterator::operator += (const size_t k)
  {
    if(_vertexP->size() < (_position + k))
    {
      throw Exception("Null pointer exception, icrement > MultiGraph-vertexes-size!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return *this;
    }
    _position += k;
    return *this;
  }

  template<typename V, typename E> inline
  typename Multigraph<V, E>::VertexIterator &Multigraph<V, E>::VertexIterator::operator -= (const size_t k)
  {
    if(_position - k < 0)
    {
      throw Exception("Null pointer exception, decrement < 0 !", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return *this;
    }
    _position -= k;
    return *this;
  }

  template<typename V, typename E> inline
  bool Multigraph<V, E>::VertexIterator::operator ==(const Multigraph<V, E>::VertexIterator &iterator)
  {
    return (_vertexP == iterator._vertexP && _position == iterator._position);
  }

  template<typename V, typename E> inline
  bool Multigraph<V, E>::VertexIterator::operator !=(const Multigraph<V, E>::VertexIterator &iterator)
  {
    return (_vertexP != iterator._vertexP || _position != iterator._position);
  }

  template<typename V, typename E> inline
  Vertex<V, E> &Multigraph<V, E>::VertexIterator::operator *()
  {
    //if(_position )
    return _vertexP->at(_position);
  }

  template<typename V, typename E> inline
  Multigraph<V, E>::Allocator::~Allocator()
  {
    returnAll();
  }

  template<typename V, typename E> inline
  Vertex<V, E> *Multigraph<V, E>::Allocator::getVertex(V& dt)
  {
    Vertex<V, E>* newVertex = new Vertex<V, E>(dt);
    vertexes_pool.push_back(newVertex);
    return newVertex;
  }

  template<typename V, typename E> inline
  Edge<V, E> *Multigraph<V, E>::Allocator::getEdge(Vertex<V, E>* src, Vertex<V, E>* dst, E value)
  {
    Edge<V, E>* newEdge = new Edge<V, E>(src, dst, value);
    edges_pool.push_back(newEdge);
    return newEdge;
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::Allocator::returnVertex(Vertex<V, E> *vertex)
  {
    auto pos = std::find(vertexes_pool.begin(), vertexes_pool.end(), vertex);

    if(pos == vertexes_pool.end())
    {
      THROW_MG_EXCEPTION("can't find returned value in vertex pool!");
      return;
    }

    vertexes_pool.erase(pos);
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::Allocator::returnEdge(Edge<V, E> *edge)
  {
    auto pos = std::find(edges_pool.begin(), edges_pool.end(), edge);

    if(pos == edges_pool.end())
    {
      THROW_MG_EXCEPTION("can't find returned value in edges pool!");
      return;
    }

    edges_pool.erase(pos);
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::Allocator::returnAll()
  {
    std::for_each(vertexes_pool.begin(), vertexes_pool.end(),
                  [](Vertex<V, E>* i) {delete i;});

    std::for_each(edges_pool.begin(), edges_pool.end(),
                  [](Edge<V, E>* i) {delete i;});
  }

} // end of namespace

#endif // MULTIGRAPH_H
