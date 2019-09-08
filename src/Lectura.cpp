#include "Lectura.hpp"
#include <fstream>
#include <string>
#include <sstream>

/**
    @author Luis Suazo
*/

using namespace std;

void LecturaNodos(const char *filename,Graph& Grafo){
    boost::iostreams::mapped_file_source mmap(filename);
    boost::iostreams::stream<boost::iostreams::mapped_file_source> fe(mmap,  boost::iostreams::mapped_file::readonly);
    std::string cadena;
    while(std::getline(fe, cadena)){
        boost::container::vector<std::string> results;
        boost::split(results, cadena, [](char c){return c == ' ';});
        uint id=stoi(results.at(0));
        double_t x=stod(results.at(1));
        double_t y=stod(results.at(2));
        results.clear();
        results.shrink_to_fit();
        Nodo *node=new Nodo(id,x,y);
        boost::container::vector<Arista*> aux;
        Grafo.emplace(node,aux);
    }
    fe.close();
    mmap.close();
}

void LecturaAristas(const char *filename,Graph& Grafo){
    boost::iostreams::mapped_file_source mmap(filename);
    boost::iostreams::stream<boost::iostreams::mapped_file_source> fe(mmap,  boost::iostreams::mapped_file::readonly);
    std::string cadena;
    boost::unordered_set<std::pair<uint,uint>> aux;
    Nodo *node=new Nodo(0,0,0);
    while(std::getline(fe, cadena)){
        boost::container::vector<std::string> results;
        boost::split(results, cadena, [](char c){return c == ' ';});
        uint idInicio=stoi(results.at(0));
        uint idFin=stoi(results.at(1));
        double_t costo=stod(results.at(2));
        node->id=idInicio;
        if(aux.find(std::make_pair(idInicio,idFin))==aux.end()){
            Grafo[node].push_back(new Arista(0,idFin,costo));
        }  
    }
    delete(node);
}

