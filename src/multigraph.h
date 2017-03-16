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

    void deleteVertex(V value);

    bool vertexIsIsolated(V value);

    void generateDotText(std::string name);
    void readFromDotFile(std::string name);

    std::vector<V> getVertexes() const;

  private:
    std::vector<Edge<V, E>> edges;
    std::vector<V> vertexes;
  };

  template<typename V, typename E> inline
  void Multigraph<V, E>::addVertex(V value)
  {
    if(std::find(vertexes.begin(), vertexes.end(), value) != vertexes.end())
    {
      throw Exception("Vertex already exist!");
      return;
    }

    vertexes.push_back(value);
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::addEdge(V src, V dst, E value)
  {
    if(src == dst)
    {
      throw Exception("The multigraph prevents the creation of loops!");
      return;
    }

    if(std::find(vertexes.begin(), vertexes.end(), src) == vertexes.end())
    {
      throw Exception("Src vertex doesn't exist!");
      return;
    }

    if(std::find(vertexes.begin(), vertexes.end(), dst) == vertexes.end())
    {
      throw Exception("Dst vertex doesn't exist!");
      return;
    }

    edges.push_back(Edge<V, E>(src, dst, value));
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::deleteVertex(V value)
  {
    auto pos = std::find(vertexes.begin(), vertexes.end(), value);

    if(pos == vertexes.end())
    {
      throw Exception("Vertex doesn't exist!");
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
      throw Exception("Vertex doesn't exist!");
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
             outputFile << i << ";";
    });
    outputFile << "\n";
    std::for_each (edges.begin(), edges.end(), [&outputFile](Edge<V, E> i)
    {
             outputFile << i.getSource()
                        << "->"
                        << i.getDestenation()
                        << "[label=\""
                        << i.getValue()
                        << "\"];";
    });
    outputFile << "\n}";
    outputFile.close();
  }

  template<typename V, typename E> inline
  void Multigraph<V, E>::readFromDotFile(std::string name)
  {

  }

  template<typename V, typename E> inline
  std::vector<V> Multigraph<V, E>::getVertexes() const
  {
    return vertexes;
  }

} // end of namespace

#endif // MULTIGRAPH_H
