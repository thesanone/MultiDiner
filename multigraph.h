#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include "edge.h"
#include "mgexception.h"

#include <vector>
#include <algorithm>

namespace mg
{
    template <typename V, typename E>
    class Multigraph
    {
    public:
        Multigraph() {}

        void addVertex(V value);
        void addEdge(V src, V dst, E value);

    private:
        std::vector<Edge<V, E>> edges;
        std::vector<V> vertexes;
    };

    template<typename V, typename E> inline
    void Multigraph<V, E>::addVertex(V value)
    {
        for(typename std::vector<V>::iterator i = vertexes.begin(); i!= vertexes.end(); i++)
        {
            if((*i) == value)
            {
                throw Exception("Person already exist!");
                return;
            }
        }

        vertexes.push_back(value);
    }

    template<typename V, typename E> inline
    void Multigraph<V, E>::addEdge(V src, V dst, E value)
    {
        //std::vector<V>::iterator it = ;
        if(std::find(vertexes.begin(), vertexes.end(), src) == vertexes.end())
        {
            throw Exception("Creditor doesn't exist!");
            return;
        }
        //std::vector<V>::iterator it2 = std::find(vertexes.begin(), vertexes.end(), src);
        if(std::find(vertexes.begin(), vertexes.end(), dst) == vertexes.end())
        {
            throw Exception("Debtor doesn't exist!");
            return;
        }
        /*for(typename std::vector<V*>::iterator i = vertexes.begin(); i!= vertexes.end(); i++)
        {
            if(*(*i) != src)
            {
                throw Exception("Creditor doesn't exist!");
                return;
            }
            if(*(*i) != dst)
            {
                throw Exception("Debtor doesn't exist!");
                return;
            }
        }*/

        edges.push_back(Edge<V, E>(src, dst, value));
    }

} // end of namespace

#endif // MULTIGRAPH_H
