#ifndef AStar_HPP
#define AStar_HPP
#include "Estructura.hpp"
#include "Funciones.hpp"

/**
    @author Luis Suazo
*/

GraphPath *AStar(uint,uint,Graph&,boost::container::vector<double_t>&);
GraphPath *AStar_2(uint,uint,Graph&,boost::container::vector<double_t>&);
                                    
#endif