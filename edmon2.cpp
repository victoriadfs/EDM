#include <iostream>
#include<stdio.h>
#include "graph2.h"
#include <cmath>
using namespace std;
//TO DO: -try catch block
//-time messen

//modifizieren von Kantengewicht fuer neuer Knoten:
void neuekantenge(vector<double>&modwe,const Graph&g,double randomgewicht){
    int neuknoten_id=g.num_nodes()-1;
    double minimum=randomgewicht;
    for(const auto& nach:g.get_node(neuknoten_id).adjacent_nodes()){//suche min
        minimum=min(minimum,modwe[nach.knummer()]);
    }
    for(const auto& nach:g.get_node(neuknoten_id).adjacent_nodes()){//subdarhiere min
        modwe[nach.knummer()]-=minimum;
    }
}

//Fuege einen Knoten hinzu und alle eingehenden und ausgehenen kanten die vom Kreis kommen wuerden
void addkreis(Graph&g,vector<int>&kreis,vector<double>&modwe,vector<bool>&exknoten){
    g.add_nodes(1);//Fuege einen Knoten hinzu
    double randomgewicht;
    int neuknoten=g.num_nodes()-1;
    for(const int& knoten: kreis){
        for(const auto& nach: g.get_node(knoten).adjacent_nodes()){//verbinde diesen knoten mit allen knoten die auch mit dem kreis verbunden waren ausser die zwischen kreismitgliedern
            if(exknoten[nach.id()]){
                g.add_edge(neuknoten,nach.id(),nach.knummer(),modwe[nach.knummer()]);
                g.add_edgeout(nach.id(),neuknoten,nach.knummer(),modwe[nach.knummer()]);
                randomgewicht=modwe[nach.knummer()];
            }
        }
        for(const auto& nach: g.get_node(knoten).adjacent_nodesout()){
            if(exknoten[nach.id()]){
                g.add_edgeout(neuknoten,nach.id(),nach.knummer(),modwe[nach.knummer()]);
                g.add_edge(nach.id(),neuknoten,nach.knummer(),modwe[nach.knummer()]);
            }
        }
    }
    exknoten.push_back(true);//Haben den Knoten zu der Liste der Exisistierenden Knoten hinzugefuegt
    neuekantenge(modwe,g,randomgewicht);//Aktualisiere Kantengewichte
}

//Suchen vom Kreis
vector<int>Kreis(int knoten,const vector<int>&F, const Graph&g){
    vector<int>kreis;
    int aktuellerknoten=knoten;
    do{ 
        aktuellerknoten=g.get_node(aktuellerknoten).adjacent_nodes()[F[aktuellerknoten]].id();
        kreis.push_back(aktuellerknoten);
    }while(aktuellerknoten!=knoten);
    return kreis;
}

vector<int> edmon(Graph &g, vector<bool> & exknoten,int root,vector<double>&modwe){
    //F bilden, also die Kanten leichteste modifizierte Kante jeweils suchen
    vector<int>F(g.num_nodes(),-1);
    for(int i=0;i<g.num_nodes();i++){
        if(exknoten[i] && i!=root){
            for(int j=0;j<g.get_node(i).adjacent_nodes().size();j++){
                int kantennummer=g.get_node(i).adjacent_nodes()[j].knummer();
                if(modwe[kantennummer]==0 && exknoten[g.get_node(i).adjacent_nodes()[j].id()]){//hierbei nur Knoten erlaubt die nicht knontrahierd wurden und bei mod kantengewicht hat leichteste kante gewicht 0
                    F[i]=j;
                    break;
                }
            }
        }
    }
    //Schauen on Arborezens <-> es gibt keinen Kreis (da der eingehende Grad von jeden Knoten auser wurzel nach knostruktion 1 ist)
    vector<bool>visited(g.num_nodes(),false);
    vector<int>kreis;
    for(int knoten=0;knoten<g.num_nodes();knoten++){
        if(F[knoten]!=-1 && exknoten[knoten]){
            int nachbar=g.get_node(knoten).adjacent_nodes()[F[knoten]].id();
            if(visited[nachbar] && nachbar!=root){
                kreis=Kreis(nachbar,F,g);
                break;
            }
           visited[nachbar]=true;
        }
    }
    if(kreis.empty()){
        return F;//Ist Arborezens sind fertig
    }
    else{
        for(const int& knoten: kreis){//elimienieren Knoten
            exknoten[knoten]=false;
        }
        int nkid=g.num_nodes();//neuer Knoten id
        addkreis(g,kreis,modwe,exknoten);//Kreis kontrahieren
        vector<int>G=edmon(g,exknoten,root,modwe);
        //G ist Abrorezens auf Knotrahierten Grafen
        //Suchen welche Kante in den Kontrahierten Kreis reingeht bzw welchen Knoten die zugehoert
        int eingehend;
        int benutztekante;
        for(const auto& knoten: kreis){
            for(int i=0;i<g.get_node(knoten).adjacent_nodes().size();i++){
                auto nach=g.get_node(knoten).adjacent_nodes()[i];
                if(nach.knummer()==g.get_node(nkid).adjacent_nodes()[G[nkid]].knummer()){
                    eingehend=knoten;
                    benutztekante=i;
                    break;
                }
            }
        }
        //Bei neuer Arborezens, werden alle Kanten aus dem Kreis benutzt auser eine, die die in den Knoten reingeht, in den G scon reingeht
        G[eingehend]=benutztekante;
        for(const int& knoten:kreis){
            if(knoten!=eingehend){
                G[knoten]=F[knoten];
            }
        }
        return G;
    }
}

void ausgeben(const Graph&g,const vector<int>&output, int n, const vector<double>&weight){
    double totalweigth=0;
    for(int i=1;i<n;i++){
        totalweigth+=weight[g.get_node(i).adjacent_nodes()[output[i]].knummer()];
    }
    cout<<totalweigth<<"\n";
    for(int i=1;i<n;i++){
        cout<<g.get_node(i).adjacent_nodes()[output[i]].knummer()<<"\n";
    }
}

void anfangedom(Graph &g,int root){
    int n=g.num_nodes();
    int m=0;
    for(int i=0;i<n;i++){
        m+=g.get_node(i).adjacent_nodes().size();
    }
    vector<bool>exknoten(n,true);//in dieser Liste werden Knoten gespeichert, welche in dem moment im (kontrahierten) Graph sind
    vector<double>modwe(m);
    vector<double>weights(m);//speichern von original gewichten
    //Kantengewichte zum ersten Mal modifizieren
    for(int i=0;i<n;i++){
        double minimum;
        bool b=false;
        for(const auto& nach: g.get_node(i).adjacent_nodes()){
            weights[nach.knummer()]=nach.edge_weight();
            if(b){
                minimum=min(minimum,nach.edge_weight());
            }
            else{
                minimum=nach.edge_weight();
                b=true;
            }
        }
        for(const auto& nach: g.get_node(i).adjacent_nodes()){
                modwe[nach.knummer()]=nach.edge_weight()-minimum;
        }
    }
    ausgeben(g,edmon(g,exknoten,root,modwe),n,weights);
    
}

int main(int argc, char* argv[]){ 
    if (argc > 1) {
        Graph g(argv[1], Graph::directed);
        anfangedom(g,0);
    }
}