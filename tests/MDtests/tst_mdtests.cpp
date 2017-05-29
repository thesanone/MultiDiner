#include <QString>
#include <QtTest>

#include "multigraph.h"
#include <string>
#include <sstream>

using namespace mg;
using namespace std;

class MDTests : public QObject
{
  Q_OBJECT

public:
  MDTests();

private Q_SLOTS:
  // vertex
  void createVertex();
  void vertexDataModify();
  void vertexAddIncomingEdge();
  void vertexAddOutgoingEdge();
  void vertexDelIncomingEdge();
  void vertexDeloutgoingEdge();

  // edge
  void createEdge();
  void edgeValueModify();

  // multigraph
  void mgEmptyCreate();
  void mgAddVertex();
  void mgAddEdge();
  void mgDelVertex();
  void mgDelEdge();
  void mgClear();
  void mgSerializeTest();
};

MDTests::MDTests()
{
}

void MDTests::createVertex()
{
  string vertexValue("Vert");
  Vertex<string, float> vert(vertexValue);
  QVERIFY(vert.getData() == vertexValue);
}

void MDTests::vertexDataModify()
{
  string vertexValue("Vert");
  string newVertexValue("HO-HO-HO");
  Vertex<string, float> vert(vertexValue);
  vert.setData(newVertexValue);
  QVERIFY(vert.getData() == newVertexValue);
}

void MDTests::vertexAddIncomingEdge()
{
  string vertexValue1("Vert1");
  string vertexValue2("Vert2");
  Vertex<string, float> vert1(vertexValue1);
  Vertex<string, float> vert2(vertexValue2);
  Edge<string, float> edge(&vert1, &vert2, 10.5f);

  vert2.addIncomingEdge(&edge);
  auto incomingEdges = vert2.getIncomingEdges();

  QVERIFY(find(incomingEdges.begin(), incomingEdges.end(), &edge) != incomingEdges.end());
}

void MDTests::vertexAddOutgoingEdge()
{
  string vertexValue1("Vert1");
  string vertexValue2("Vert2");
  Vertex<string, float> vert1(vertexValue1);
  Vertex<string, float> vert2(vertexValue2);
  Edge<string, float> edge(&vert1, &vert2, 10.5f);

  vert1.addOutgoingEdge(&edge);
  auto outgoingEdges = vert1.getOutgoingEdges();

  QVERIFY(find(outgoingEdges.begin(), outgoingEdges.end(), &edge) != outgoingEdges.end());
}

void MDTests::vertexDelIncomingEdge()
{
  string vertexValue1("Vert1");
  string vertexValue2("Vert2");
  Vertex<string, float> vert1(vertexValue1);
  Vertex<string, float> vert2(vertexValue2);
  Edge<string, float> edge(&vert1, &vert2, 10.5f);
  Edge<string, float> edge2(&vert1, &vert2, 21.5f);

  vert2.addIncomingEdge(&edge);
  vert2.addIncomingEdge(&edge2);
  vert2.delIncomingEdge(&edge);
  auto incomingEdges = vert2.getIncomingEdges();

  QVERIFY(find(incomingEdges.begin(), incomingEdges.end(), &edge) == incomingEdges.end()
          && find(incomingEdges.begin(), incomingEdges.end(), &edge2) != incomingEdges.end());
}

void MDTests::vertexDeloutgoingEdge()
{
  string vertexValue1("Vert1");
  string vertexValue2("Vert2");
  Vertex<string, float> vert1(vertexValue1);
  Vertex<string, float> vert2(vertexValue2);
  Edge<string, float> edge(&vert1, &vert2, 10.5f);
  Edge<string, float> edge2(&vert1, &vert2, 21.5f);

  vert1.addOutgoingEdge(&edge);
  vert1.addOutgoingEdge(&edge2);
  vert1.delOutgoingEdge(&edge);
  auto outgoingEdges = vert1.getOutgoingEdges();

  QVERIFY(find(outgoingEdges.begin(), outgoingEdges.end(), &edge) == outgoingEdges.end()
          && find(outgoingEdges.begin(), outgoingEdges.end(), &edge2) != outgoingEdges.end());
}

void MDTests::createEdge()
{
  float edgeValue = 10.5f;
  string vertexValue1("Vert1");
  string vertexValue2("Vert2");
  Vertex<string, float> vert1(vertexValue1);
  Vertex<string, float> vert2(vertexValue2);
  Edge<string, float> edge(&vert1, &vert2, edgeValue);
  QVERIFY(edge.getSource() == &vert1 &&
          edge.getDestenation() == &vert2 &&
          edge.getValue() == edgeValue);
}

void MDTests::edgeValueModify()
{
  float newEdgeValue = -0.5f;
  string vertexValue1("Vert1");
  string vertexValue2("Vert2");
  Vertex<string, float> vert1(vertexValue1);
  Vertex<string, float> vert2(vertexValue2);
  Edge<string, float> edge(&vert1, &vert2, 10.5f);
  edge.setValue(newEdgeValue);
  QVERIFY(edge.getValue() == newEdgeValue);
}

void MDTests::mgEmptyCreate()
{
  Multigraph<string, float> graph;
  QVERIFY(graph.getVertexes().empty());
}

void MDTests::mgAddVertex()
{
  Multigraph<string, float> graph;
  string vertexValue("Vert");
  graph.addVertex(vertexValue);
  auto vertexes = graph.getVertexes();
  QVERIFY(find_if(vertexes.begin(), vertexes.end(), [=](Vertex<string, float> *i)
  {
    return i->getData() == vertexValue;
  }) != vertexes.end());
}

void MDTests::mgAddEdge()
{
  Multigraph<string, float> graph;
  graph.addVertex("Vert1");
  graph.addVertex("Vert2");
  graph.addEdge("Vert1", "Vert2", 10.5f);
  auto vertexes = graph.getVertexes();
  QVERIFY(find_if(vertexes.begin(), vertexes.end(), [](Vertex<string, float> *i)
  {
    return i->getData() == "Vert1" && i->getOutgoingEdges().front()
        && i->getOutgoingEdges().front()->getValue() == 10.5f;
  }) != vertexes.end());
}

void MDTests::mgDelVertex()
{
  Multigraph<string, float> graph;
  graph.addVertex("Vert1");
  graph.addVertex("Vert2");
  graph.deleteVertex("Vert1");
  auto vertexes = graph.getVertexes();
  QVERIFY(
      find_if(vertexes.begin(), vertexes.end(), [](Vertex<string, float> *i)
      {
        return i->getData() == "Vert1";
      }) == vertexes.end()
      && find_if(vertexes.begin(), vertexes.end(), [](Vertex<string, float> *i)
      {
        return i->getData() == "Vert2";
      }) != vertexes.end());
}

void MDTests::mgDelEdge()
{
  Multigraph<string, float> graph;
  graph.addVertex("Vert1");
  graph.addVertex("Vert2");
  graph.addEdge("Vert1", "Vert2", 10.5f);
  graph.addEdge("Vert2", "Vert1", 5.f);
  graph.deleteEdge("Vert1", "Vert2", 10.5f);

  auto vertexes = graph.getVertexes();
  QVERIFY(
    find_if(vertexes.begin(), vertexes.end(), [](Vertex<string, float> *i)
    {
      return i->getData() == "Vert1" && i->getOutgoingEdges().front()
          && i->getOutgoingEdges().front()->getValue() == 10.5f;
    }) == vertexes.end()
    && find_if(vertexes.begin(), vertexes.end(), [](Vertex<string, float> *i)
    {
      return i->getData() == "Vert2" && i->getOutgoingEdges().front()
          && i->getOutgoingEdges().front()->getValue() == 5.f;
    }) != vertexes.end()
  );
}

void MDTests::mgClear()
{
  Multigraph<string, float> graph;
  graph.addVertex("Vert1");
  graph.addVertex("Vert2");
  graph.addEdge("Vert1", "Vert2", 10.5f);
  graph.addEdge("Vert2", "Vert1", 5.f);

  graph.clear();

  QVERIFY(graph.getVertexes().empty());
}

void MDTests::mgSerializeTest()
{
  Multigraph<string, float> graph;
  graph.addVertex("Vert1");
  graph.addVertex("Vert2");
  graph.addEdge("Vert1", "Vert2", 10.5f);
  graph.addEdge("Vert2", "Vert1", 5.f);


  ostringstream _ostream;
  _ostream << graph;

  graph.clear();

  istringstream _istream(_ostream.str());
  _istream >> graph;

  auto vertexes = graph.getVertexes();
  QVERIFY(
    find_if(vertexes.begin(), vertexes.end(), [](Vertex<string, float> *i)
    {
      return i->getData() == "Vert1" && i->getOutgoingEdges().front()
          && i->getOutgoingEdges().front()->getValue() == 10.5f;
    }) != vertexes.end()
    && find_if(vertexes.begin(), vertexes.end(), [](Vertex<string, float> *i)
    {
      return i->getData() == "Vert2" && i->getOutgoingEdges().front()
          && i->getOutgoingEdges().front()->getValue() == 5.f;
    }) != vertexes.end()
  );
}




QTEST_APPLESS_MAIN(MDTests)

#include "tst_mdtests.moc"
