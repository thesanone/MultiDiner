#ifndef EDGE_H
#define EDGE_H

template <typename V, typename E>
class Edge
{
public:
  Edge() {}
  Edge(V src, V dst, E value):destenation(dst), source(src), value(value)
  {
    /*this->source = src;
    this->destenation = dst;
    this->value = value;*/
  }

  V getDestenation() const {return destenation;}
  //void setDestenation(const V &value) {destenation = value;}

  V getSource() const {return source;}
  //void setSource(const V &value) {source = value;}

  E getValue() const {return value;}
  void setValue(const E &value) {this->value = value;}

private:
  V destenation;
  V source;
  E value;
};

#endif // EDGE_H
