// graph.h (Declaration of Class Graph)
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
//ACHTUNG: Graphklasse verändert, sodass die Klasse Neighbor jetzt die Knoten speichern von denen unser Knoten aus erreichtbar
//d.h. in v.adjacent_nodes() werden die Knoten x gespeichert, sodass eine Kante (x,v) existiert
//print Fkt wurde nicht geandert und ist damit schwachsinn

class Graph {
public:
  using NodeId = int;  // vertices are numbered 0,...,num_nodes()-1

  class Neighbor {
  public:
        Neighbor(Graph::NodeId n, double w,int nummer);
        double edge_weight() const;
        Graph::NodeId id() const;
        int knummer() const;
  private:
        Graph::NodeId _id;
        double _edge_weight;
        int _knummer; //welche Kantennummer entspricht diese Kante 
    };

  class Node {
  public:
        void add_neighbor(Graph::NodeId nodeid, double weight, int nummer);
        const std::vector<Neighbor> & adjacent_nodes() const;
  private:
        std::vector<Neighbor> _neighbors;
    };

  enum DirType {directed, undirected};  // enum defines a type with possible values
  Graph(NodeId num_nodes, DirType dirtype);
  Graph(char const* filename, DirType dirtype);

  void add_nodes(NodeId num_new_nodes);
  void add_edge(NodeId tail, NodeId head,int nummer, double weight = 1.0);

  NodeId num_nodes() const;
  const Node & get_node(NodeId) const;
  void print() const;

  const DirType dirtype;
  static const NodeId invalid_node;
  static const double infinite_weight;

private:
  std::vector<Node> _nodes;
};

#endif // GRAPH_H
