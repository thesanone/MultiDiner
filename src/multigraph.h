#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include "vertex.h"
#include "edge.h"
#include "mgexception.h"

#include <list>
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
    void deleteEdge(V src, V dst, E value);

    void generateDotText(std::string name);

    std::list<Vertex<V, E> *> getVertexes() const;

    class VertexIterator
    {
    public:
      VertexIterator(const VertexIterator& iterator);
      VertexIterator(const size_t position, std::list<Vertex<V, E>*>* vertex);
      VertexIterator& operator ++ ();
      VertexIterator& operator += (const size_t k);
      bool operator == (const VertexIterator& iterator);
      bool operator != (const VertexIterator& iterator);
      Vertex<V, E>* operator * ();

    private:
      size_t _position;
      std::list<Vertex<V, E>*>* _vertexP;
      typename std::list<Vertex<V, E>*>::iterator _vertexIterator;
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
      std::list<Vertex<V, E>*> vertexes_pool;
      std::list<Edge<V, E>*> edges_pool;
    };

    Allocator alloc;

  protected:
    std::list<Vertex<V, E>*> vertexes;
  };


  // ********************************************************************************************
  // *********************************** impelementatin *****************************************
  // ********************************************************************************************


  template <typename V, typename E>
  std::ostream& operator<< (std::ostream& os, const Multigraph<V, E>& dt)
  {
    os << dt.vertexes.size() << "\n";
    size_t edgesCounter = 0;
    std::for_each (dt.vertexes.begin(), dt.vertexes.end(), [&os, &edgesCounter](Vertex<V, E>* i)
    {
             os << i->getData() << "\n";
             edgesCounter += i->getOutgoingEdges().size();
    });

    os << edgesCounter << "\n";

    std::for_each (dt.vertexes.begin(), dt.vertexes.end(), [&os](Vertex<V, E>* i)
    {
      auto outgoingEdges = i->getOutgoingEdges();

      std::for_each(outgoingEdges.begin(), outgoingEdges.end(), [&os](Edge<V, E>* j)
      {
        os << j->getSource()->getData() << "\n"
           << j->getDestenation()->getData() << "\n"
           << j->getValue() << "\n";
      });
    });

    return os;
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
       dt.addVertex(obj);
     }

     is >> edgesSize;
     V obj2;
     E valueObj;
     for(size_t i = 0; i != edgesSize; i++)
     {
       is >> obj
          >> obj2
          >> valueObj;
       dt.addEdge(obj, obj2, valueObj);
     }

     return is;
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::addVertex(V value)
  {
    auto vertexPos = std::find_if(vertexes.begin(), vertexes.end(), [value](Vertex<V, E>* i)
    {
      return i->getData() == value;
    });

    if(vertexPos != vertexes.end())
    {
      THROW_MG_EXCEPTION("Vertex already exist!");
      return;
    }

    auto newVertex = alloc.getVertex(value);
    vertexes.push_back(newVertex);
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::addEdge(V src, V dst, E value)
  {
    if(src == dst)
    {
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
      THROW_MG_EXCEPTION("Src vertex doesn't exist!");
      return;
    }

    if(!dstPointer)
    {
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
    auto vertexPos = std::find_if(
          vertexes.begin(), vertexes.end(), [value](Vertex<V, E>* i)
    {
      return i->getData() == value;
    });

    if(vertexPos == vertexes.end())
    {
      THROW_MG_EXCEPTION("Vertex doesn't exist!");
      return;
    }

    Vertex<V, E>* vertexPointer = *vertexPos;

    auto vertexIncomingEdges = vertexPointer->getIncomingEdges();
    auto vertexOutgoingEdges = vertexPointer->getOutgoingEdges();

    std::for_each(vertexIncomingEdges.begin(), vertexIncomingEdges.end(),
                  [this](Edge<V, E>* i)
    {
      i->getSource()->delOutgoingEdge(i);
      i->getDestenation()->delIncomingEdge(i);
      alloc.returnEdge(i);
    });

    std::for_each(vertexOutgoingEdges.begin(), vertexOutgoingEdges.end(),
                  [this](Edge<V, E>* i)
    {
      i->getDestenation()->delIncomingEdge(i);
      i->getSource()->delOutgoingEdge(i);
      alloc.returnEdge(i);
    });

    vertexes.erase(vertexPos);

    alloc.returnVertex(vertexPointer);
  }

  template<typename V, typename E> inline
  bool Multigraph<V, E>::vertexIsIsolated(V value)
  {
    auto vertexPos = std::find_if(
              vertexes.begin(), vertexes.end(), [value](Vertex<V, E>* i)
        {
          return i->getData() == value;
        });

    if(vertexPos == vertexes.end())
    {
      THROW_MG_EXCEPTION("Vertex doesn't exist!");
      return false;
    }

    Vertex<V, E>* vertexPointer = *vertexPos;

    auto vertexIncomingEdges = vertexPointer->getIncomingEdges();
    auto vertexOutgoingEdges = vertexPointer->getOutgoingEdges();

    return (vertexIncomingEdges.empty() &&
            vertexOutgoingEdges.empty());
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::deleteEdge(V src, V dst, E value)
  {
    auto srcPos = std::find_if(vertexes.begin(), vertexes.end(),
                               [src](Vertex<V, E>* i)
    {
      return i->getData() == src;
    });

    if(srcPos == vertexes.end())
    {
      THROW_MG_EXCEPTION("src vertex doesn't exist!");
      return;
    }

    Vertex<V, E>* srcPointer = *srcPos;

    auto outgoingEdges = srcPointer->getOutgoingEdges();

    auto edgePos = std::find_if(outgoingEdges.begin(), outgoingEdges.end(),
                               [dst, value](Edge<V, E>* i)
    {
      return (i->getDestenation()->getData() == dst)
          && (i->getValue() == value);
    });

    if(edgePos == outgoingEdges.end())
    {
      THROW_MG_EXCEPTION("edge doesn't exist");
      return;
    }

    Edge<V, E>* edgePointer = *edgePos;

    // удаление дуги
    srcPointer->delOutgoingEdge(edgePointer);
    edgePointer->getDestenation()->delIncomingEdge(edgePointer);
    alloc.returnEdge(edgePointer);
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

      std::for_each(outgoingEdges.begin(), outgoingEdges.end(), [&outputFile](Edge<V, E>* j)
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
  std::list<Vertex<V, E> *> Multigraph<V, E>::getVertexes() const
  {
    return vertexes;
  }

  template<typename V, typename E> inline
  Multigraph<V, E>::VertexIterator::VertexIterator(const Multigraph<V, E>::VertexIterator &iterator)
  {
    _vertexP = iterator._vertexP;
    _position = iterator._position;
    _vertexIterator = _vertexP->begin();
    for(size_t i = 0; i < _position; i++) _vertexIterator++;
  }

  template<typename V, typename E> inline
  Multigraph<V, E>::VertexIterator::VertexIterator(const size_t position, std::list<Vertex<V, E>*> *vertex)
  {
    _position = position;
    _vertexP = vertex;
    _vertexIterator = _vertexP->begin();
    for(size_t i = 0; i < _position; i++) _vertexIterator++;
  }

  template<typename V, typename E>
  typename Multigraph<V, E>::VertexIterator &Multigraph<V, E>::VertexIterator::operator ++()
  {
    if(_vertexP->size() < (_position + 1))
    {
      THROW_MG_EXCEPTION("Null pointer exception, icrement > MultiGraph-vertexes-size!");
      return *this;
    }
    _position++;
    _vertexIterator++;
    return *this;
  }

  template<typename V, typename E> inline
  typename Multigraph<V, E>::VertexIterator &Multigraph<V, E>::VertexIterator::operator += (const size_t k)
  {
    if(_vertexP->size() < (_position + k))
    {
      THROW_MG_EXCEPTION("Null pointer exception, icrement > MultiGraph-vertexes-size!");
      return *this;
    }
    _position += k;
    for(size_t i = 0; i < k; i++) _vertexIterator++;
    return *this;
  }

  template<typename V, typename E> inline
  bool Multigraph<V, E>::VertexIterator::operator ==(const Multigraph<V, E>::VertexIterator &iterator)
  {
    return (_vertexIterator == iterator._vertexIterator);
  }

  template<typename V, typename E> inline
  bool Multigraph<V, E>::VertexIterator::operator !=(const Multigraph<V, E>::VertexIterator &iterator)
  {
    return (_vertexIterator != iterator._vertexIterator);
  }

  template<typename V, typename E> inline
  Vertex<V, E> *Multigraph<V, E>::VertexIterator::operator *()
  {
    if(_position == _vertexP->size())
    {
      THROW_MG_EXCEPTION("Null pointer exception, Try access to last NULL element!");
      return NULL;
    }
    return *_vertexIterator;
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
    delete vertex;
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
    delete edge;
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::Allocator::returnAll()
  {
    std::for_each(vertexes_pool.begin(), vertexes_pool.end(),
                  [](Vertex<V, E>* i) {delete i;});

    std::for_each(edges_pool.begin(), edges_pool.end(),
                  [](Edge<V, E>* i) {delete i;});
    vertexes_pool.clear();
    edges_pool.clear();
  }

} // end of namespace

#endif // MULTIGRAPH_H
