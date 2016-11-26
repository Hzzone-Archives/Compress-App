//
// Created by Hzzone on 2016/10/30.
//

#ifndef PROJECT1_COMPONENT_H
#define PROJECT1_COMPONENT_H
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <map>
#include <cstring>
#include <stdlib.h>
#include <algorithm>
using namespace std;

template <typename E>
class HuffNode {
public:
    virtual ~HuffNode(){}
    virtual int weight() = 0;
    virtual bool isLeaf() = 0;
};


template <typename E>
class LeafNode: public HuffNode<E>{
private:
    E it;
    int wgt;
public:
    LeafNode(const E& val, int freq){it=val; wgt=freq;}
    int weight(){return wgt;}
    E val(){return it;}
    bool isLeaf(){return true;}
};


template <typename E>
class IntlNode: public HuffNode<E>{
private:
    HuffNode<E>* lc;
    HuffNode<E>* rc;
    int wgt;
public:
    IntlNode(HuffNode<E>* l, HuffNode<E>* r){
        wgt = l->weight()+r->weight(); lc = l; rc = r;
    }
    int weight(){
        return wgt;
    }
    bool isLeaf(){return false;}
    HuffNode<E>* left(){return lc;}
    HuffNode<E>* right(){return rc;}
    void setLeft(HuffNode<E>* b) {
        lc = (HuffNode<E>*)b;
    }
    void setRight(HuffNode<E>* b) {
        rc = (HuffNode<E>*)b;
    }
};

template <typename E>
class HuffTree {
private:
    HuffNode<E>* Root;
public:
    HuffTree(E const& val, int freq){Root = new LeafNode<E>(val, freq);}
    HuffTree(HuffTree<E>* l, HuffTree<E>* r){Root = new IntlNode<E>(l->root(), r->root());}
    ~HuffTree(){}
    HuffNode<E>* root(){return Root;}
    int weight() const {return Root->weight();}
};


void encode(char* input_filename, char* output_filename, char* secret, const char* fileFormat);
void decode(char* input_filename, char* output_filename);
bool isCompressFile(char* filename);
const char* isSecretFile(char* filename);
const char* fileFormat(char* filename);
#endif //PROJECT1_COMPONENT_H
