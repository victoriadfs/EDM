// testgraph.cpp (Read Digraph from File and Print)
#include <iostream>
#include<stdio.h>
#include "graph2.h"
#include <cmath>
using namespace std;
void aktualisieren(const Graph &g,vector<double>&modw,int knoten, double minw){
    for(auto nach:g.get_node(knoten).adjacent_nodes()){
         modw[nach.knummer()]=nach.edge_weight()-minw;
    }
}
void pathkonstruieren(const Graph &g, vector <bool> &visited,vector <pair<int,pair<int,int> > >&path, vector <double> &modw, int neuKnoten, int &anfang){
    if(!visited[neuKnoten]){
        visited[neuKnoten]=true;
        if(!g.get_node(neuKnoten).adjacent_nodes().empty()){
        double minweight=g.get_node(neuKnoten).adjacent_nodes()[0].edge_weight();
        int minweightid=0;
        for(int i=1;i<g.get_node(neuKnoten).adjacent_nodes().size();i++){
             if(g.get_node(neuKnoten).adjacent_nodes()[i].edge_weight()<minweight){
                 minweight=g.get_node(neuKnoten).adjacent_nodes()[i].edge_weight();
                 minweightid=i;
            }
        }
        aktualisieren(g,modw,neuKnoten,minweight);
        path.push_back(make_pair(neuKnoten,make_pair(neuKnoten,minweightid)));
        if(g.get_node(neuKnoten).adjacent_nodes()[minweightid].id()!=0){//Die wurzel nicht der nachste knoten
            pathkonstruieren(g,visited,path,modw,g.get_node(neuKnoten).adjacent_nodes()[minweightid].id(),anfang);
        }
        else{//falls der vorganger der Wurzelknoten ist dann kann nicht viel passieren
            anfang=path.size();
        }
    }
    else{
        int pathid;
        for(int i=0;i<path.size();i++){
            if(path[i].first==neuKnoten){
                pathid=i;
                break;
            }
        }
        if(pathid<anfang){
            return;//Hier ist kein Kreis entstanden
        }
        else{//Hier entsteht ein Kreis
            
        }
    }

    }
}
int main(int argc, char* argv[])
{ 
    if (argc > 1) {
        Graph g(argv[1], Graph::directed);
        //g.print();
        int m=0;
        for(int i=0;i<g.num_nodes();i++){
            m+=g.get_node(i).adjacent_nodes().size();
        }
        vector<bool>selfloop(m,false);//schauen ob selfloops
        vector<double>modwe(m);//specher von modified weights
        for(int i=0;i<g.num_nodes();i++){
            if(!g.get_node(i).adjacent_nodes().empty()){
                double minw=g.get_node(i).adjacent_nodes()[0].edge_weight();
                for(auto nach:g.get_node(i).adjacent_nodes()){
                    minw=min(minw,nach.edge_weight());
                }
                aktualisieren(g,modwe,i,minw);
            }
        }
        int anfang=0; //moment mit dem ein Kreis gebildet wird 
        vector <pair<int,pair<int,int> > > path; //knotenid + (knotenid von reinkommenden, in dem dann der id in der Liste vom edge)
        vector <bool> visited(g.num_nodes(),false);//wollen alle Knoten im path einfuegen
        for(int i=1; i<g.num_nodes();i++){
            if(!visited[i]){

            }
        }
    }
}
