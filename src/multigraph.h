#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

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

    void deleteVertex(Vertex<V, E> value);
    bool vertexIsIsolated(Vertex<V, E> value);

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
      Vertex<V, E>& operator * ();

    private:
      size_t _position;
      std::vector<Vertex<V, E>*>* _vertexP;
    };

    VertexIterator beginV() {return VertexIterator(0, &vertexes);}
    VertexIterator endV() {return VertexIterator(vertexes.size(), &vertexes);}

  private:
    template <typename V2, typename E2>
    friend std::ostream& operator<< (std::ostream& os, const Multigraph<V2, E2>& dt);

    template <typename V2, typename E2>
    friend std::istream& operator>> (std::istream& is, Multigraph<V2, E2>& dt);

  protected:
    std::vector<Vertex<V, E>> vertexes;
  };


  // ********************************************************************************************
  // *********************************** impelementatin *****************************************
  // ********************************************************************************************


  template <typename V, typename E>
  std::ostream& operator<< (std::ostream& os, const Multigraph<V, E>& dt)
  {
    os << dt.vertexes.size() << "\n";
    std::for_each(dt.vertexes.begin(), dt.vertexes.end(), [&os](V i){ os << i << "\n"; });

    os << dt.edges.size() << "\n";
    std::for_each(dt.edges.begin(), dt.edges.end(), [&os](Edge<V, E> i)
    {
      os << i.getSource() << "\n"
         << i.getDestenation() << "\n"
         << i.getValue() << "\n";
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
    if(std::find(vertexes.begin(), vertexes.end(), value) != vertexes.end())
    {
      throw Exception("Vertex already exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return;
    }

    vertexes.push_back(value);
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::addEdge(V src, V dst, E value)
  {
    if(src == dst)
    {
      throw Exception("The multigraph prevents the creation of loops!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return;
    }

    if(std::find(vertexes.begin(), vertexes.end(), src) == vertexes.end())
    {
      throw Exception("Src vertex doesn't exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return;
    }

    if(std::find(vertexes.begin(), vertexes.end(), dst) == vertexes.end())
    {
      throw Exception("Dst vertex doesn't exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return;
    }

    edges.push_back(Edge<V, E>(src, dst, value));
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::clear()
  {
    vertexes.clear();
    edges.clear();
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::deleteVertex(Vertex<mg::V, mg::E> value)
  {
    auto pos = std::find(vertexes.begin(), vertexes.end(), value);

    if(pos == vertexes.end())
    {
      throw Exception("Vertex doesn't exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return;
    }

    vertexes.erase(pos);

    typename std::vector<Edge<V, E>>::iterator sec_pos =
        std::remove_if(edges.begin(), edges.end(), [value] (Edge<V, E> i)
        {
            return (i.getSource() == value || i.getDestenation() == value);
        });

    edges.erase(sec_pos, edges.end());
  }

  template<typename V, typename E> inline
  bool Multigraph<V, E>::vertexIsIsolated(V value)
  {
    if(std::find(vertexes.begin(), vertexes.end(), value) == vertexes.end())
    {
      throw Exception("Vertex doesn't exist!", __LINE__, __FUNCTION__, __TIMESTAMP__);
      return true;
    }

    typename std::vector<Edge<V, E>>::iterator pos =
        std::find_if(edges.begin(), edges.end(), [value] (Edge<V, E> i)
        {
            return (i.getSource() == value || i.getDestenation() == value);
        });
    return pos == edges.end();
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::generateDotText(std::string name)
  {
    std::ofstream outputFile;
    outputFile.open(name);
    outputFile << "digraph {\n";
    std::for_each (vertexes.begin(), vertexes.end(), [&outputFile](V i)
    {
             outputFile <<"\"" << i <<"\"" << ";\n";
    });
//    outputFile << "\n";
    std::for_each (edges.begin(), edges.end(), [&outputFile](Edge<V, E> i)
    {
             outputFile <<"\"" << i.getSource() <<"\""
                        << "->"
                        <<"\"" << i.getDestenation() <<"\""
                        << "[label=\""
                        << i.getValue()
                        << "\"];\n";
    });
    outputFile << "}";
    outputFile.close();
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::readFromDotFile(std::string name)
  {

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
  Multigraph<V, E>::VertexIterator::VertexIterator(const size_t position, std::vector<V> *vertex)
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

} // end of namespace

#endif // MULTIGRAPH_H
