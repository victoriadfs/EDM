// graph.cpp (Implementation of Class Graph)

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include "graph2.h"

const Graph::NodeId Graph::invalid_node = -1;
const double Graph::infinite_weight = std::numeric_limits<double>::max();


void Graph::add_nodes(NodeId num_new_nodes)
{
    _nodes.resize(num_nodes() + num_new_nodes);
}

Graph::Neighbor::Neighbor(Graph::NodeId n, double w, int nummer): _id(n), _edge_weight(w),_knummer(nummer){
}

Graph::Graph(NodeId num, DirType dtype): dirtype(dtype), _nodes(num) {}

void Graph::add_edge(NodeId tail, NodeId head, int nummer, double weight)
{
    if (tail >= num_nodes() or tail < 0 or head >= num_nodes() or head < 0) {
        throw std::runtime_error("Edge cannot be added due to undefined endpoint.");
    }
    _nodes[tail].add_neighbor(head, weight, nummer);
    if (dirtype == Graph::undirected) {
        _nodes[head].add_neighbor(tail, weight, nummer);
    }
}

void Graph::Node::add_neighbor(Graph::NodeId nodeid, double weight, int nummer)
{
    _neighbors.push_back(Graph::Neighbor(nodeid, weight, nummer));
}

const std::vector<Graph::Neighbor> & Graph::Node::adjacent_nodes() const
{
    return _neighbors;
}

Graph::NodeId Graph::num_nodes() const
{
    return _nodes.size();
}

const Graph::Node & Graph::get_node(NodeId node) const
{
    if (node < 0 or node >= static_cast<int>(_nodes.size())) {
        throw std::runtime_error("Invalid nodeid in Graph::get_node.");
    }
    return _nodes[node];
}

Graph::NodeId Graph::Neighbor::id() const
{
    return _id;
}

double Graph::Neighbor::edge_weight() const
{
    return _edge_weight;
}


int Graph::Neighbor::knummer() const
{
    return _knummer;
}

void Graph::print() const
{
    if (dirtype == Graph::directed) {
        std::cout << "Digraph ";
    } else {
        std::cout << "Undirected graph ";
    }
    std::cout << "with " << num_nodes() << " vertices, numbered 0,...,"
              << num_nodes() - 1 << ".\n";

    for (auto nodeid = 0; nodeid < num_nodes(); ++nodeid) {
        std::cout << "The following edges are ";
        if (dirtype == Graph::directed) {
            std::cout << "leaving";
        } else {
            std::cout << "incident to";
        }
        std::cout << " vertex " << nodeid << ":\n";
        for (auto neighbor: _nodes[nodeid].adjacent_nodes()) {
            std::cout << nodeid << " - " << neighbor.id()
                      << " weight = " << neighbor.edge_weight() << "\n";
        }
    }
}

Graph::Graph(char const * filename, DirType dtype): dirtype(dtype)
{
    std::ifstream file(filename);                             // open file
    if (not file) {
        throw std::runtime_error("Cannot open file.");
    }

    Graph::NodeId num = 0;
    std::string line;
    std::getline(file, line);                 // get first line of file
    std::stringstream ss(line);               // convert line to a stringstream
    ss >> num;                                // for which we can use >>
    if (not ss) {
        throw std::runtime_error("Invalid file format.");
    }
    add_nodes(num);
    int nummer=0;//Nummerierung
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Graph::NodeId head, tail;
        ss >> tail >> head;
        if (not ss) {
            throw std::runtime_error("Invalid file format.");
        }
        double weight = 1.0;
        ss >> weight;
        //std::cout<<tail<<" "<<head<<" "<<nummer<<"\n";
        if (tail != head) {
            add_edge(head, tail, nummer, weight);//ACHTUNG GRAPH WURDE UMGEDREHT
        }
        else {
            throw std::runtime_error("Invalid file format: loops not allowed.");
        }
        nummer++;
    }
}

