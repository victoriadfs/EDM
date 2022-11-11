//Programm soll das Zentrum eines Baumes bestimmen 
//Algorithmus basierend auf Beweis VL zum finden eines Zentrums in einem Baum
/*Bemerkung zur Kompilierung:
Folgende Komandos wurden benutzt (Hier am Beispiel mit inst8_):
clang++ -std=c++11 -o test baumzentrum2.cpp graph.cpp
./test inst8_*/
#include <iostream>
#include<stdio.h>
#include<queue>
#include "graph.h"
//TO_DO: mirar que la laufzeit sea adecuada

using namespace std;

void zentrumbestimmen(const Graph &g){
    int n=g.num_nodes();
    vector<int>grad(n,0); //Speichern Grad jedes Knoten
    queue<pair<int,int>>schlange; //Speichern Blaetter_id,Phase elimination
    for(int i=0;i<n;i++){
        grad[i]=g.get_node(i).adjacent_nodes().size();
        if(grad[i]==1)schlange.push(make_pair(i,0));
    }
    //Beginne ein modifiziertes BFS
    vector<bool>visited(g.num_nodes(),false);
    //Modifiziertes BFS
    while(n>2){ //jeder Knoten kommt genau ein mal in der Schleife vor 
        int knoten=schlange.front().first;
        int nextphase=schlange.front().second+1;//geben zuruck was fuer ein Level die naechsten eliminierten Knoten haben werden
        schlange.pop();
        n--;
        visited[knoten]=true;
        for(auto neighbor : g.get_node(knoten).adjacent_nodes()){//jede Kante wird hoechstens 2 Mal aufgerufen
            int n_id=neighbor.id();
            grad[n_id]--;//entferne der Kante 
            if(!visited[n_id] and grad[n_id]==1){//wenn der Nachbar zum Blatt geworden ist fuege hinzu
                schlange.push(make_pair(n_id,nextphase));
            }
        }
    }
    //Nur zwei Knoten wurden nicht eliminiert,aber sie sind in der queue denn alle ihre Nachbarn wurden eliminiert ausser vllt einen und damit sind sie Blaetter
    int poswurzel1=schlange.front().first;
    int phase1=schlange.front().second;
    schlange.pop();
    int poswurzel2=schlange.front().first;
    int phase2=schlange.front().second;
    if(phase1==phase2)cout<<poswurzel1<<" "<<poswurzel2;//wenn im selben eliminaritionschritt dann sind beide Wurzeln
    else cout<<poswurzel2;//sonst ist der letzte die Wurzel, denn die Phasen werden geordnet im queue aufgerufen
}

int main(int argc, char* argv[])
{   if (argc > 1) {
        Graph g(argv[1], Graph::undirected);
        //g.print();
        zentrumbestimmen(g);
    }
}