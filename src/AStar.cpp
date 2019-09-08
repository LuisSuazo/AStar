#include "Estructura.hpp"
#include "BinaryHeap.hpp"
#include "Funciones.hpp"

using namespace std;

/**
    @author Luis Suazo
*/

/* Astar con una fibonacci heap de Boost*/ 
GraphPath *AStar(uint Start,uint Goal,Graph& Grafo, boost::container::vector<double_t>& hC){
    boost::heap::fibonacci_heap<Expandido*, boost::heap::compare<Expandido::CompareKey> > Open;
    boost::unordered_map<uint,open_handle> Handles;
    Expandido *Opn=new Expandido(Start,0,0);
    auto pointer=Open.emplace(Opn);
    boost::unordered_map<uint,uint> Padre;
    double_t *gScore=new double_t[Grafo.size()+1];
    uint *Close=new uint[Grafo.size()+1];
    for(uint i=0;i<=Grafo.size();++i){
        gScore[i]=std::numeric_limits<uint>::max();
        Close[i]=0;
    }
    Handles.emplace(Opn->id,pointer);
    Nodo *fin=new Nodo(Goal,0,0);
    auto b=Grafo.find(fin);
    delete(fin);
    Nodo *node=new Nodo(0,0,0);
    uint expandidos=0;
    while(!Open.empty()){
        Opn=Open.top(); Open.pop();
        Close[Opn->id]=1;
        Handles.erase(Opn->id);
        if(Opn->id==Goal){
            GraphPath *path=new GraphPath();
            buildPath(Start,Goal,Padre,path->path,Grafo);
            path->expandidos=expandidos;
            path->weight=Opn->g;
            Padre.clear();
            delete(Opn);
            while(!Open.empty()){
                Opn=Open.top();
                Open.pop();
                delete(Opn);
            }
            return path;            
        }
        node->id=Opn->id;
        auto a=Grafo.find(node);
        expandidos++;
        for(auto const &it : Grafo[node]){
            if(it->idNodo ==Opn->id ) continue;
            uint id=it->idNodo;
            double_t g=it->costo+Opn->g;
            double_t h=0;
            /*Euclidiana*/ 
            h=distEuclidiana(a->first,b->first);
            /*Perfecta*/
            //h=hC[id-1];
            if(g < gScore[id]){
                gScore[id] = g;
                Padre[id]=Opn->id;  
                Open.emplace(new Expandido(id,g,h));
            }
        }
        delete(Opn);
    }
    Padre.clear();
    delete(node);
    GraphPath *path=new GraphPath();
    return path;
}

/* Astar con una Binary Heap propia*/ 
GraphPath *AStar_2(uint Start,uint Goal,Graph& Grafo, boost::container::vector<double_t>& hC){
    BinaryHeap *Open=new BinaryHeap(HEAPSIZE3);
    boost::unordered_map<uint,uint> Padre;
    double_t *gScore=new double_t[Grafo.size()+1];
    uint *Close=new uint[Grafo.size()+1];
    for(uint i=0;i<=Grafo.size();++i){
        gScore[i]=std::numeric_limits<uint>::max();
        Close[i]=0;
    }
    Expandido *Opn=new Expandido(Start,0,0);
    Open->insert(Opn);
    /*Nodo *fin=new Nodo(Goal,0,0);
    auto b=Grafo.find(fin);
    delete(fin);
    */
    Nodo *node=new Nodo(0,0,0);
    uint expandidos=0;
    while(Open->top() !=NULL){
        Opn=Open->pop();
        Close[Opn->id]=1;
        if(Opn->id==Goal){
            GraphPath *path=new GraphPath();
            buildPath(Start,Goal,Padre,path->path,Grafo);
            path->expandidos=expandidos;
            path->weight=Opn->g;
            Padre.clear();
            delete(Opn);
            while(Open->top() !=NULL){
                Opn=Open->pop();
                delete(Opn);
            }
            return path;         
        }
        node->id=Opn->id;
        //auto a=Grafo.find(node);
        expandidos++;
        for(auto const &it : Grafo[node]){
            uint id=it->idNodo;
            double_t g=it->costo+Opn->g;
            double_t h=0;
            /* Euclidiana */ 
            /*
            h=distEuclidiana(a->first,b->first);
            */
            /*Perfecta*/
            h=hC[id-1];
            if(g < gScore[id]){
                gScore[id] = g;
                Padre[id]=Opn->id; 
                Open->insert(new Expandido(id,g,h));
            }
        }
        delete(Opn);
    }
    Padre.clear();
    delete(Opn);
    delete[](gScore);
    delete[](Close);
    delete(node);
    GraphPath *path=new GraphPath();
    return path;
}

