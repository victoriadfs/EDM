//Programm soll das Zentrum eines Baumes bestimmen 
//Algorithmus basierend auf Beweis VL zum finden eines Zentrums in einem Baum
#include <iostream>
#include<stdio.h>
#include<queue>
#include "graph.h"
//TO_DO: mirar que la laufzeit sea adecuada

using namespace std;

void zentrumbestimmen(Graph g){
    int n=g.num_nodes();
    vector<int>grad(n,0); //Speichern Grad jedes Knoten
    queue<int>schlange; //Speichern Blaetter
    for(int i=0;i<n;i++){
        grad[i]=g.get_node(i).adjacent_nodes().size();
        if(grad[i]==1)schlange.push(i);
    }
    //Beginne ein modifiziertes BFS
    vector<bool>visited(g.num_nodes(),false);
    vector<int>level(n,0);//in welchem Schritt von entferne alle Blaetter wird der jeweilige Knoten entfernt
    int levelzentrum=0;// wird als letztes entfernt
    while(!schlange.empty()){ //jeder Knoten kommt genau ein mal in der Schleife vor 
        int knoten=schlange.front();
        schlange.pop();
        visited[knoten]=true;
        for(auto neighbor : g.get_node(knoten).adjacent_nodes()){
            int n_id=neighbor.id();
            grad[n_id]--;//entferne den Knoten
            if(!visited[n_id] and grad[n_id]==1){//wenn der Nachbar zum Blatt geworden ist fuege hinzu
                schlange.push(n_id);
                level[n_id]=level[knoten]+1;
                levelzentrum=max(level[n_id],levelzentrum);
            }
        }
    }
    for(int i=0;i<n;i++){
        if(level[i]==levelzentrum)cout<<i<<" ";
    }
}

int main(int argc, char* argv[])
{   if (argc > 1) {
        Graph g(argv[1], Graph::undirected);
        //g.print();
        zentrumbestimmen(g);
    }
}