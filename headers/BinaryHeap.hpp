#ifndef BinaryHeap_HPP
#define BinaryHeap_HPP

#include "Estructura.hpp"

struct BinaryHeap{
    lint Size;
    lint statPercolated;
    Expandido **Elements;

    explicit BinaryHeap (uint MaxElements) {
        this->Elements = (Expandido**)malloc(sizeof(Expandido*)*MaxElements+2);
        this->Size=0;
        this->statPercolated=0;
    }

    void percolateUP(uint hole, Expandido *tmp){
        if (this->Size != 0){
            for (; hole > 1 && tmp->key < this->Elements[hole/2]->key; hole /= 2){
                this->Elements[hole] = this->Elements[hole/2];
                this->Elements[hole]->heapindex = hole;
                ++this->statPercolated;
            }  
            this->Elements[hole] = tmp;
            this->Elements[hole]->heapindex = hole;
        }
    }

    void percolateDown(uint hole, Expandido *tmp){
        uint child;
        if (this->Size != 0){
            for (; 2*hole <= this->Size; hole = child){
            child = 2*hole;
            if (child != this->Size &&  this->Elements[child+1]->key <  this->Elements[child]->key)
                ++child;
            if ( this->Elements[child]->key < tmp->key){
                this->Elements[hole] =  this->Elements[child];
                this->Elements[hole]->heapindex = hole;
                ++this->statPercolated;
            }else
                break;
            }
            this->Elements[hole] = tmp;
            this->Elements[hole]->heapindex = hole;
        }
    }

    void percolateUD(uint hole, Expandido *tmp){
        if (this->Size != 0){
            if (hole > 1 && this->Elements[hole/2]->key > tmp->key)
                percolateUP(hole, tmp);
            else
                percolateDown(hole, tmp);
        }
    }

    void deleteHeap(Expandido *thiscell){
        if (thiscell->heapindex != 0){
            percolateUD(thiscell->heapindex, this->Elements[this->Size--]);
            thiscell->heapindex = 0;
        }
    }

    Expandido *top(){
        if (this->Size == 0)
            return NULL;
        return this->Elements[1];
    }

    Expandido *pop(){
        Expandido *thiscell;
        if (this->Size == 0)
            return NULL;
        thiscell = this->Elements[1];
        thiscell->heapindex = 0;
        percolateDown(1, this->Elements[this->Size--]);
        return thiscell;
    }

    int size(){
        return this->Size;
    }

    Expandido *posheap(int i){
        return this->Elements[i];
    }

    void insert(Expandido *thiscell){
        if (thiscell->heapindex == 0)
            percolateUP(++this->Size, thiscell);
        else
            percolateUD(thiscell->heapindex, this->Elements[thiscell->heapindex]);
    }

};

#endif
