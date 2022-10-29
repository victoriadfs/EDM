//Programm soll das Zentrum eines Baumes bestimmen 
//Algorithmus basierend auf Programm Blatt 2 Aufgabe 2b)
#include <iostream>
#include<stdio.h>
#include<queue>
#include "graph.h"
//TO_DO: 1.mirar las abschaetzungen con los nodos 2.quitar los cout que sobran

using namespace std;
void bfs(Graph g,int wurzel,vector<int>& parent,vector<int>& aufruf){
    queue<int>schlange;
    vector<bool>visited(g.num_nodes(),false); //Schauen ob wir den Knoten schon besucht haben
    int n=0;
    schlange.push(wurzel);
    int knoten;
    while(!schlange.empty()){
        knoten=schlange.front();
        schlange.pop();
        aufruf[n]=knoten;
        n+=1;
        //cout<<knoten<<"\n";
        visited[knoten]=true;
        for(auto neighbor : g.get_node(knoten).adjacent_nodes()){
            int n_id=neighbor.id();
            if(!visited[n_id]){
                //cout<<n_id<<"\n";
                schlange.push(n_id);
                parent[n_id]=knoten;
            }
        }
    }
}


void zentrumbestimmen(Graph g){
    vector<int>parentid(g.num_nodes(),0);//Merken uns Vater
    vector<int>aufruf(g.num_nodes());//Merken uns in welcher Reihenfolge die Knoten aufgetaucht sind im BFS in aufruf[0]=wurzel, dann etc.
    bfs(g,0,parentid,aufruf);
    vector<int>numchild(g.num_nodes(),0);
    int zk1,zk2,knoten,n=g.num_nodes();
    bool b=false;//sagt ob es einen zweiten ZK gibt
    //cout<<n;
    for(int i=n-1;i>=0;i--){
        knoten=aufruf[i];
        numchild[parentid[knoten]]+=numchild[knoten]+1;
        //cout<<knoten<<" "<<numchild[knoten]<<"\n";
        if(numchild[knoten]>=n/2-1){//Hier muss man noch schauen ob es sinvoll ist mit type int diese abschaetzung noch gilt
            zk1=knoten;
            if(numchild[knoten]<=n/2){
                zk2=parentid[zk1];
                b=true;
            }
            break;
        }
    }
    if(b){
        cout<<"Es gibt zwei Zentralknoten: "<<zk1<<" "<<zk2<<"\n";
    }
    else{
        cout<<"Es gibt einen Zentralknoten: "<<zk1;
    }

}
int main(int argc, char* argv[])
{   if (argc > 1) {
        Graph g(argv[1], Graph::undirected);
        //g.print();
        zentrumbestimmen(g);
    }
}