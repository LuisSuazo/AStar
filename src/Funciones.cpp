#include "Funciones.hpp"
#include "BinaryHeap.hpp"

/**
    @author Luis Suazo
*/

/*Para la heuristica perfecta*/
void dijkstraBoost(uint Start,graph_t& Grafo,boost::container::vector<vertex_descriptor> &p,weight_map_t& WeightMap
                                    ,boost::container::vector<double_t>& d){
    vertex_descriptor s =  boost::vertex(Start-1, Grafo);
    boost::dijkstra_shortest_paths(
        Grafo, s,boost::predecessor_map(boost::make_iterator_property_map(p.begin(), boost::get(boost::vertex_index, Grafo)))
        .distance_map(boost::make_iterator_property_map(d.begin(), boost::get(boost::vertex_index, Grafo)))
        .weight_map(WeightMap));
}

/*Para la heuristica Euclidiana*/
double_t distEuclidiana(Nodo *a, Nodo *b){
    return sqrt(pow((b->x-a->x),2)+pow((b->y-a->y),2));
}

/*Para la heuristica perfecta, pero programada*/
boost::container::vector<double_t> dijkstra(uint Start,Graph& Grafo){
    boost::heap::fibonacci_heap<Expandido*, boost::heap::compare<Expandido::CompareKey> > Open;
    boost::unordered_map<uint,open_handle> Handles;
    Expandido *Opn=new Expandido(Start,0,0);
    auto pointer=Open.emplace(Opn);
    boost::unordered_map<uint,uint> Padre;
    boost::container::vector<double_t> gScore(Grafo.size()+1);
    uint *Close=new uint[Grafo.size()+1];
    for(uint i=0;i<=Grafo.size();++i){
        gScore[i]=std::numeric_limits<uint>::max();
        Close[i]=0;
    }
    Handles.emplace(Opn->id,pointer);
    Nodo *node=new Nodo(0,0,0);
    while(!Open.empty()){
        Opn=Open.top(); Open.pop();
        Close[Opn->id]=1;
        Handles.erase(Opn->id);
        node->id=Opn->id;
        for(auto const &it : Grafo[node]){
            if(it->idNodo ==Opn->id ) continue;
            uint id=it->idNodo;
            double_t g=it->costo+Opn->g;
            double_t h=0;
            if(g < gScore[id]){
                gScore[id] = g;
                Padre.emplace(id,Opn->id);  
                Open.emplace(new Expandido(id,g,h));
            }
        }
        delete(Opn);
    }
    delete(node);
    Padre.clear();
    return gScore;
}

void buildPath(int Start,int Goal,
                      boost::unordered_map<uint,uint>& Padres,
                      boost::container::vector<Nodo*>& Nodos,Graph& g){
    auto auxVertex=Padres.find(Goal);
    Nodo *encontrar=new Nodo(auxVertex->first,0,0);
    auto find=g.find(encontrar);
    Nodos.push_back(find->first);
    int termino=0;
    while(termino!=Start){
        auxVertex=Padres.find(auxVertex->second);
        encontrar->id=auxVertex->first;
        find=g.find(encontrar);
        Nodos.insert(Nodos.begin(),find->first);
        termino=auxVertex->second;
    } 
}

graph_t graph2boostGraph(Graph& Grafo){
    std::vector<std::pair<uint, uint>> edge_array;
    std::vector<Arista> weights;
    graph_t g( Grafo.size());
    for(auto it= Grafo.begin();it!=Grafo.end();++it){
        for(uint i=0;i<it->second.size();++i){
            //edge_array.push_back(std::make_pair(it->first-1,it->second.at(i).first-1));
            edge_array.push_back(std::make_pair(it->first->id-1,it->second.at(i)->idNodo-1));
            //cout<<it->second.at(i).second->costo2<<endl;
            weights.push_back(Arista(it->second.at(i)->idArista,it->second.at(i)->idNodo,it->second.at(i)->costo));
        }
    }
    for(uint i=0;i<edge_array.size();i++){
        boost::add_edge(edge_array.at(i).first,edge_array.at(i).second, weights.at(i),g);
    }
    edge_array.clear();
    edge_array.shrink_to_fit();
    weights.clear();
    weights.shrink_to_fit();
    return g;
}

// https://stackoverflow.com/questions/669438/how-to-get-memory-usage-at-runtime-using-c
void process_mem_usage(double& vm_usage, double& resident_set){
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}