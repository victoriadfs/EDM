#include <iostream>
#include <stdio.h>
#include <cmath>
#include <queue>
#include <list>
#include "graph.h"
//TO -Do: mirar si empieza con 0 o con uno la enumeriacion de los edges
//        mirar si los excesos en push estan bien cambiados
//        mirar si hay manera mas smart de hacer las aktualizaciones en maxhoh
//bemerkung siempre me interesan los ausgehende Knoten asi que bien

//Wollen uberprufen ob t von s aus erreichtbar ist, sonst gibt es keinen Fluss
bool exfluss(const Graph& g, const int& s, const int & t, const int& n){
    std::vector <bool> visited(n,false);
    std::queue <int> schlange;
    schlange.push(s);
    while(!schlange.empty()){
        int aktuellerknoten = schlange.front();
        schlange.pop();
        visited[aktuellerknoten] = true;
        if(aktuellerknoten == t){
            break;
        }
        for(const auto& kanten: g.get_node(aktuellerknoten).adjacent_nodes()){
            int nachbar = kanten.head_id();
            if(!visited[nachbar]){
                schlange.push(nachbar);
            }
        }
    }
    return visited[t];
}


void print(const std::vector <double> &fluss, const int& m, const double& valf){
    std::cout<<"Maxflowwert: "<<valf<<"\n";
    std::cout<<"Flussoutput: "<<"\n";
    for(int i=0; i<m; i++){
        std::cout<<i<<": "<<fluss[i]<<"\n";
    }
}

//Wir inserten die Rueckkanten, zu denen gegben wir als Gewicht die Vorkante
//Um auf deren Gewicht zuzugreifen kann man das durch den aktuellenFluss machen
//man kann rueckkanten daran erkennen, dass deren Indize >=m ist
void insertrueckkanten(Graph & g, std:: vector <double> cap, std:: vector <double> & ex, double &valf){
    for(int i = 0; i < g.num_nodes() ;i++){
        for(const auto& kante: g.get_node(i).adjacent_nodes()){
            int kante_id = kante.edge_id();
            g.add_edge(kante.head_id(),i , kante_id);
            cap[kante_id] = kante.edge_weight();
            ex[kante.head_id()]+=kante.edge_weight();
            valf += kante.edge_weight();
        }
    }
}

void initialisierung(const Graph & g,const int & s, const int & n,const int & m,std::vector <int> &dist,std::vector <double> & fluss, std:: vector <std::list<int> > & aktiveknoten){
    //Distanzmarkierung:
    dist.assign(n,0);
    dist[s] = n;
    //Fluss, Aktive Knoten (nur Knoten mit s verbunden sind aktiv und haben distanz 0)
    for (const auto& kante: g.get_node(s).adjacent_nodes()){
            fluss[kante.edge_id()] = kante.edge_weight();
            aktiveknoten[0].emplace_front(kante.head_id());
    }
    //Am Anfang sind keine Kanten zulassig da alle Knoten auf dem selben Level sind
}

void push(const Graph & g, const int & m, const int & knoten,const int & kante, std::vector <double> & fluss, std::vector <std::list<int> > & aktiveknoten, std::vector <std::list<int> > zulassigekanten, const std::vector <double> & cap, std::vector <double> & ex, const std::vector <int> & dist, int & maxhohe, double &valf){
    bool rueckkante = false;
    double uf;
    int vorkanteid;
    int kante_id = g.get_node(knoten).adjacent_nodes()[kante].edge_id();
    int nachkonten = g.get_node(knoten).adjacent_nodes()[kante].head_id();
    if (kante_id >= m){
        rueckkante = true;
    }

    if (rueckkante){
        vorkanteid = g.get_node(knoten).adjacent_nodes()[kante].edge_weight();
        uf = fluss[vorkanteid];
    }
    else {
        uf = cap[kante_id]-fluss[kante_id];
    }

    double y = std::min(uf,ex[knoten]);
    bool saturierend = false;
    bool gleich = false;
    if (y == uf) saturierend = true;
    if (ex[knoten] == y) gleich = true;

    //Augmentierung
    if (rueckkante){
        fluss[vorkanteid] -= y;
        valf -= y;
    }
    else {
        fluss[kante_id] += y;
        valf += y;
    }

    //Aktualisieren von exp und aktive Knoten
    ex[knoten] -= y;
    if (!saturierend || gleich) {
        aktiveknoten[dist[knoten]].remove(knoten);
    }
    if (ex[nachkonten] == 0){
        aktiveknoten[dist[nachkonten]].emplace_front(nachkonten);
    }
    ex[nachkonten] += y;

    //Aktualisierung von zulassige Kanten
    if (saturierend){
        zulassigekanten[knoten].remove(kante); //Wenn der Push saturierend ist die Kante nicht mehr im Restgraphen
    }

    //Aktualisierung von maxhohe
    bool maxhohegefunden = false;
    while (!maxhohegefunden){
        if (!aktiveknoten[maxhohe].empty() || maxhohe == 0){
            maxhohegefunden = true;
        }
        else {
            maxhohe -= 1;
        }
    }
}

void relable (const Graph & g, const int& knoten, std:: vector <int>& dist, std:: vector <std::list<int> >& aktiveknoten, int & maxhohe, std:: vector <double> & fluss, std:: vector <double> &cap, std::vector <std::list<int> >& zulassigekanten){
    int neuehohe=g.num_nodes()*2;
    for (const auto & kante: g.get_node(knoten).adjacent_nodes()){
        neuehohe=std::min(neuehohe, dist[kante.head_id()]);
    }
    //Aktualisiere Liste Aktiver Knoten, Distanzmarkierung, maxhohe eines aktiven knoten
    aktiveknoten[dist[knoten]].remove(knoten);
    dist[knoten] = neuehohe + 1;
    aktiveknoten[dist[knoten]].emplace_front(knoten);
    maxhohe = dist[knoten];

    //Aktualisierung der LIste der zulassigen Kanten
    for (int i=0; i < g.get_node(knoten).adjacent_nodes().size(); i++){
        int nachknoten = g.get_node(knoten).adjacent_nodes()[i].head_id();
        if (dist[nachknoten] == neuehohe){
            zulassigekanten[knoten].push_back(i);
        }
    }
}

void Goldberg(Graph &g){
    const int s = 0, t = 1;
    const int n = g.num_nodes();
    const int m = g.num_edges();
    if (exfluss(g, s, t, n)){
        std::cout<<"Maxflowwert: 0"<<"\n";
    }
    else{
    //Initialisierung vom den Hilfvariablen:
    //Aktueller Fluss:
    std::vector <double> fluss(m,0);
    //Aktive Knoten:
    std::vector <std::list<int> > aktiveknoten(2*n);
    //zulassige Kanten:
    std::vector <std::list<int> > zulassigekanten(n); //hier wird der id in der adjacenzliste gespeichert
    //distanzmarkierung
    std::vector <int> dist;
    //maximaler aktiver Knoten hohe:
    int maxhohe = 0;
    initialisierung(g, s, n, m, dist, fluss, aktiveknoten);

    double valf = 0;
    std::vector <double> ex(n,0);
    std::vector <double> cap(m);
    //Rueckkanten hinzugefuegt, die Kapazitat der Kanten gespeichert und excess initialisiert
    insertrueckkanten(g, cap, ex, valf);

    //Ablauf des Algorithmus
    while (!aktiveknoten[maxhohe].empty()){
        int akknoten = aktiveknoten[maxhohe].front();
        if (zulassigekanten[akknoten].empty()){
            relable(g, akknoten, dist, aktiveknoten, maxhohe, fluss, cap, zulassigekanten);
        }
        else{
            int kante = zulassigekanten[akknoten].front();
            push(g, m, akknoten, kante, fluss, aktiveknoten, zulassigekanten, cap, ex, dist, maxhohe, valf);
        }

    }

    //Ausdrucken vom Fluss
    print(fluss, m, valf);
    }

}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        try {
            Graph g(argv[1], Graph::directed);
            Goldberg(g);
        }
        catch (const std::runtime_error& error) {
            std::cout << "Runtime error: " << error.what() << "\n";
        }
    }
}