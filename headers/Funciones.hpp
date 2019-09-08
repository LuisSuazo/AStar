#ifndef FUNCIONES_HPP
#define FUNCIONES_HPP
#include "Estructura.hpp"

/**
    @author Luis Suazo
*/

graph_t graph2boostGraph(Graph&);
void process_mem_usage(double&, double&);
void buildPath(int Start,int Goal,
                      boost::unordered_map<uint,uint>& Padres,
                      boost::container::vector<Nodo*>& Nodos,Graph& g);
double_t distEuclidiana(Nodo *a, Nodo *b);
boost::container::vector<double_t> dijkstra(uint Start,Graph& Grafo);
void dijkstraBoost(uint,graph_t&,boost::container::vector<vertex_descriptor> &,weight_map_t&,boost::container::vector<double_t>& );
#endif