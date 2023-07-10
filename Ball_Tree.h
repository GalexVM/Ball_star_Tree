//
// Created by USER on 16/06/2023.
//

#ifndef BALL_TREE_CLION_BALL_TREE_H
#define BALL_TREE_CLION_BALL_TREE_H

#include <utility>
#include<vector>
#include<cmath>
#include "Ball_Tree_Node.h"

template<int ndim>
class Ball_Tree
{
public:
    Ball_Tree(){
        root = new Ball_Tree_Node<ndim>();
    }
    ~Ball_Tree() {
        delete root;
    }
    void bulkData(vector<Point<ndim>> data); //Insertar datos al arbol
    void printToFile();
    vector<Point<ndim>> KNN(Point<ndim>& target,int k);
    vector<Point<ndim>> KNN_lineal(Point<ndim>& target,int k);

private:
    int numNodes = 1;
    Ball_Tree_Node<ndim>* root;
    void WriteCircles(Ball_Tree_Node<ndim>* n);
};

template<int ndim>
vector<Point<ndim>> Ball_Tree<ndim>::KNN_lineal(Point<ndim> &target, int k) {
    return root->KNN_lineal(target,k);
}


template<int ndim>
vector<Point<ndim>> Ball_Tree<ndim>::KNN(Point<ndim> &target, int k) {
    vector<std::pair<Point<ndim>,double>> psin;
    vector<std::pair<Point<ndim>,double>> t = root->KNN(target,psin,root,k);

    vector<Point<ndim>> v(t.size());
    std::transform(t.begin(),t.end(),v.begin(),[](const std::pair<Point<ndim>,int> p){
        return p.first;
    });
    reverse(v.begin(), v.end());
    return v;
}

template<int ndim>
void Ball_Tree<ndim>::bulkData(vector<Point<ndim>> data) {
    root->build(data);
}



/*Extra*/

template<int ndim>
void Ball_Tree<ndim>::printToFile() {
    std::ofstream archivo("../textfiles/circles.txt", std::ios::trunc);
    if (archivo.is_open()) {
        archivo.close();
    } else {
        std::cout << "No se pudo abrir el archivo\n";
    }
    WriteCircles(root);
    root->writeDataToFile("../textfiles/data_g.txt");
}

template<int ndim>
void Ball_Tree<ndim>::WriteCircles(Ball_Tree_Node<ndim> *n) {
    n->writeCircleToFile("../textfiles/circles.txt");
    if(!n->isLeaf)
    {
        WriteCircles(n->leftChild);
        WriteCircles(n->rightChild);
    }
    else
        return;
}

#endif //BALL_TREE_CLION_BALL_TREE_H
