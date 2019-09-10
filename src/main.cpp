#include "Estructura.hpp"
#include "Lectura.hpp"
#include "Funciones.hpp"
#include "AStar.hpp"

/**
    @author Luis Suazo
*/

using namespace std;
using namespace std::chrono;
using namespace boost;

int main(int narc,char **argv){
    srand (time(NULL));
    Graph Grafo;
    LecturaNodos(argv[1],Grafo);
    LecturaAristas(argv[2],Grafo);
    graph_t gBoost=graph2boostGraph(Grafo);
    weight_map_t WeightMap_c = boost::get(&Arista::costo,gBoost);
    boost::container::vector<vertex_descriptor> c(boost::num_vertices(gBoost));
    boost::container::vector<double_t> h(boost::num_vertices(gBoost));
    uint Start=stoi(argv[3]);
    uint Goal=stoi(argv[4]);
    dijkstraBoost(Goal,gBoost,c,WeightMap_c,h);
    cout<<"Start "<<Start<<" Goal "<<Goal<<endl;
    high_resolution_clock::time_point inicio = high_resolution_clock::now();
    GraphPath *path1=AStar_2(Start,Goal,Grafo,h);
    high_resolution_clock::time_point fin = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(fin - inicio);
    cout<<"ASTAR Binary;valor:"<<path1->weight<<";path size:"<<path1->path.size()<<";expandidos:"<<path1->expandidos<<";tiempo:"<<time_span.count()<<endl;
    inicio = high_resolution_clock::now();
    GraphPath *path2=AStar(Start,Goal,Grafo,h);
    fin = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(fin - inicio);
    cout<<"ASTAR Fibo progra;valor:"<<path2->weight<<";path size:"<<path2->path.size()<<";expandidos:"<<path2->expandidos<<";tiempo:"<<time_span.count()<<endl;
    inicio = high_resolution_clock::now();
    GraphPath *path3=AStar_3(Start,Goal,Grafo,h);
    fin = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(fin - inicio);
    cout<<"ASTAR Fibo Boost;valor:"<<path3->weight<<";path size:"<<path3->path.size()<<";expandidos:"<<path3->expandidos<<";tiempo:"<<time_span.count()<<endl;
    c.clear();
    c.shrink_to_fit();
    h.clear();
    h.shrink_to_fit();
    return 0;
}
