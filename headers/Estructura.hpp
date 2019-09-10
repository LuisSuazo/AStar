#ifndef Estructura_HPP
#define Estructura_HPP

/* STL Include */
#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <iterator>

/* C Include */
#include <cstring>
#include <cmath>
#include <omp.h>

/* Boost Include */
#include <boost/unordered_map.hpp>
#include <boost/container/map.hpp>
#include <boost/config.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/unordered_set.hpp>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/numeric.hpp>
#include <boost/functional/hash.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/transform_value_property_map.hpp>
#include <boost/iostreams/stream.hpp>             
#include <boost/iostreams/device/mapped_file.hpp> 
#include <boost/algorithm/string.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/lexical_cast.hpp>

typedef unsigned int uint;
typedef unsigned long int lint;
#define HEAPSIZE 10000000


/**
    @author Luis Suazo
*/

struct Nodo{
    uint id;
    double_t x;
    double_t y;

	Nodo(uint id,double_t x,double_t y){
		this->id=id;
		this->x=x;
		this->y=y;
	}

    Nodo & operator=(const Nodo & rhs) {
		if (this == &rhs) return *this;		
		this->id = rhs.id;
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	};

	std::size_t hash_value(const Nodo &a){
		std::size_t seed=0;
		boost::hash_combine(seed,a.id);
		return seed;
	}

	bool operator==(const Nodo & rhs) const {
		return (this==&rhs);
	};

	struct NodoHasher{
		std::size_t operator()(const Nodo* rhs) const{
			std::size_t seed=0;
			boost::hash_combine(seed,boost::hash_value(rhs->id));
			return seed;
  		}
	};

	struct NodoEqual{
		bool operator() (Nodo const* t1, Nodo const* t2) const{
			return t1->id==t2->id;
		}
	};

};



struct Arista{
	uint idArista;
	uint idNodo;
    double_t costo;

	explicit Arista(uint idArista,uint idNodo,double_t costo){
		this->idArista=idArista;
		this->idNodo=idNodo;
		this->costo=costo;
	}

	Arista(){
		this->idArista=0;
		this->idNodo=0;
		this->costo=0;
	}

	~Arista(){};

	struct AristaHasher{
		std::size_t operator()(const Arista *rhs) const{
			std::size_t seed=0;
			boost::hash_combine(seed,boost::hash_value(rhs->idArista));
			return seed;
		}
	};

	struct AristaEqual{
		bool operator() (Arista const* t1, Arista const* t2) const{
			return t1->idArista==t2->idArista;
		}
	};

};

struct Expandido{
    uint id;
    double_t f;
	double_t g;
	double_t h;
	double_t key;
	Expandido* parent; 
    Expandido* child; 
    Expandido* left; 
    Expandido* right; 
	int heapindex;
    char mark; 
    char c;  

	Expandido(uint id, double_t g,double_t h){
		this->id=id;
		this->f=g+h;
		this->g=g;
		this->h=h;
		this->heapindex=0;
		this->key=this->f*1000000+this->h;
	}

	void addValues(uint id, double_t g,double_t h){
		this->id=id;
		this->f=g+h;
		this->g=g;
		this->h=h;
		this->heapindex=0;
		this->key=this->f*1000000+this->h;
	}

	~Expandido(){};

	bool operator==(const Expandido &rhs) const {
		return (this==&rhs);
	};

	struct ExpandidoHasher{
		std::size_t operator()(const Expandido *rhs) const{
			std::size_t seed=0;
			boost::hash_combine(seed,boost::hash_value(rhs->id));
			return seed;
		}
	};

	struct ExpandidoEqual{
		bool operator() (Expandido const *t1, Expandido const *t2) const{
			return (t1->id==t2->id);
		}
	};

	struct CompareKey{	
    	bool operator()(const Expandido *n1, const Expandido *n2) const{
			return n1->key>n2->key;
    	}
	};


};

struct GraphPath{
	double_t weight;
	boost::container::vector<Nodo*> path;
	uint expandidos;

	GraphPath(double_t weight,boost::container::vector<Nodo*> path,uint expandido){
		this->weight=weight;
		this->path.insert(this->path.begin(),path.begin(),path.end());
		this->expandidos=expandido;
	}

	GraphPath(){
		this->weight=0;
		this->expandidos=0;
	}

	~GraphPath(){
		this->path.clear();
		this->path.shrink_to_fit();
	};
};


struct Compare{	
    bool operator()(const Expandido& n1, const Expandido& n2) const{
		return (n1.f>n2.f);
    }
};



/*Grafo*/
typedef boost::unordered_map<Nodo*, boost::container::vector<Arista*>,Nodo::NodoHasher,Nodo::NodoEqual> Graph;

/*Handle fibonacci heap boost*/
typedef boost::heap::fibonacci_heap<Expandido*, boost::heap::compare<Expandido::CompareKey> >::handle_type open_handle;

/*Grafo para comprobar ciclos*/
typedef boost::unordered_map<uint,std::vector<uint>> Graphwp;

/*Grafo de Boost*/
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, Arista> graph_t;
typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits < graph_t >::edge_descriptor edge_descriptor;
typedef boost::property_map<graph_t, double_t Arista::*>::type weight_map_t;

#endif