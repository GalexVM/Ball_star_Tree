//
// Created by USER on 16/06/2023.
//

#ifndef BALL_TREE_CLION_BALL_TREE_NODE_H
#define BALL_TREE_CLION_BALL_TREE_NODE_H

#include <utility>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <chrono>
#include <iomanip>
#include <queue>
#include "pca.h"
#include "Point.h"
#define INF 9999999

using std::vector;
using std::cout;
using std::endl;
using std::pair;

template<int ndim>
class Ball_Tree_Node
{
public:
    bool isLeaf = false;
    bool isRoot = true;
    vector<Point<ndim>> data;
    Ball_Tree_Node<ndim>* leftChild = nullptr, * rightChild = nullptr, * parent = nullptr;
    Ball_Tree_Node() {center = new Point<ndim>;}
    ~Ball_Tree_Node() { delete leftChild; delete rightChild; }
    bool canDivide();
    void build(vector<Point<ndim>>& data_t);
    void coverData();
    void coverData2(); //Actual
    void splitData();
    void saveData(vector<Point<ndim>>& data_t);
    vector<std::pair<Point<ndim>,double>> KNN(Point<ndim>& target, vector<std::pair<Point<ndim>,double>>  psIn,
                                              Ball_Tree_Node<ndim> *node, int k);
    vector<Point<ndim>> KNN_lineal(Point<ndim>& target,int k);

    /*Impresiones*/
    void writeCircleToFile(const std::string& file);
    void writeDataToFile(const std::string& file);
    void writeExtremePoints(vector<Point<ndim>> maxp, vector<Point<ndim>> minp, Point<ndim> cen );


private:
    struct DistanceComparator {
        bool operator()(const pair<Point<ndim>, double>& p1, const pair<Point<ndim>, double>& p2) {
            return p1.second < p2.second;
        }
    };
    Point<ndim>* center;
    double radius = 0;
    int maxPoints = 1000;
    int currentPoints = 0;
    Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> stdToEigen(vector<Point<ndim>>& v);
    void sort_vectors(vector<Point<ndim>>& v1, vector<Point<ndim>>& v2);
    void sort_vectors2(Eigen::Matrix<double,Eigen::Dynamic,1>& v1, vector<Point<ndim>>& v2); //Actual
    double calculateDistMaxCurrent( int k, Point<ndim>& target, vector<std::pair<Point<ndim>,double>>& psIn, double posibility);
    double calculateDistMaxCurrent( int k, Point<ndim>& target, vector<std::pair<Point<ndim>,double>>& psIn);
    double calculateDistMinNode(Point<ndim>& target, Ball_Tree_Node<ndim>* node);
    vector<Ball_Tree_Node<ndim>*> getAllLeaves();
};

template<int ndim>
vector<Ball_Tree_Node<ndim> *> Ball_Tree_Node<ndim>::getAllLeaves() {
    vector<Ball_Tree_Node<ndim>*> r;
    Ball_Tree_Node<ndim>* temp = nullptr;
    if(!isLeaf){
        temp = leftChild;
        vector<Ball_Tree_Node<ndim>*> t = temp->getAllLeaves();
        r.insert(r.begin(),t.begin(),t.end());
        temp = rightChild;
        t = temp->getAllLeaves();
        r.insert(r.begin(),t.begin(),t.end());
        return r;
    }else{
        temp = this;
        r.push_back(temp);
        return  r;
    }
}

template<int ndim>
vector<Point<ndim>> Ball_Tree_Node<ndim>::KNN_lineal(Point<ndim> &target, int k) {
    /*
     * Idea: ubicar todos los nodos hojas
     * Buscar en los datos de todos los nodos de forma lineal
     * Sorprendentemente rápido
     * */
    vector<Point<ndim>> knn;
    std::priority_queue<pair<Point<ndim>, double>, vector<pair<Point<ndim>, double>>, DistanceComparator> pq;
    vector<Ball_Tree_Node<ndim>*> leaves = getAllLeaves();
    for(auto ball : leaves){
        for ( auto& point : ball->data) {
            double distance = point.dist(&target);
            if (pq.size() < k) {
                pq.push({point, distance});
            } else if (distance < pq.top().second) {
                pq.pop();
                pq.push({point, distance});
            }
        }

    }
    knn.reserve(k);
    while (!pq.empty()) {
        knn.push_back(pq.top().first);
        pq.pop();
    }
    reverse(knn.begin(), knn.end());
    return knn;
}

template<int ndim>
double Ball_Tree_Node<ndim>::calculateDistMinNode(Point<ndim> &target, Ball_Tree_Node<ndim>* node) {
    if (node->isRoot)
        return std::max(target.dist(node->center) - node->radius, 0.0);
    else
        return std::max(target.dist(node->center) - node->radius, calculateDistMinNode(target,node->parent));
}

template<int ndim>
double Ball_Tree_Node<ndim>::calculateDistMaxCurrent(int k, Point<ndim> &target, vector<std::pair<Point<ndim>,double>>& psIn, double posibility) {
    if(psIn.size() < k)
        return INF;
    else{
        return posibility;
    }
}
template<int ndim>
double Ball_Tree_Node<ndim>::calculateDistMaxCurrent(int k, Point<ndim> &target, vector<std::pair<Point<ndim>,double>>& psIn) {
    if(psIn.size() < k)
        return INF;
    else{
        double maxDist = 0;
        for(auto x : psIn)
        {
            double temp = x.first.dist(&target);
            if(temp > maxDist)
                maxDist = temp;
        }
        return maxDist;
    }
}

template<int ndim>
vector<std::pair<Point<ndim>,double>> Ball_Tree_Node<ndim>::KNN(Point<ndim>& target, vector<std::pair<Point<ndim>,double>> psIn,
                                                                Ball_Tree_Node<ndim> *node, int k) {
    /*
     * Idea:
     * Basado en el algoritmo de Liu et al. "New algorithms for efficient high-dimensional nonparametric classification"
     * Buscar KNN en el nodo actual
     * Si el KNN actual más lejano está más lejos que el nodo más cercano, ampliar los KNN con los datos de dicho nodo.
     * */
    vector<std::pair<Point<ndim>,double>> psOut;

    double distMaxCurrent = calculateDistMaxCurrent(k, target, psIn);
    double distMinNode = calculateDistMinNode(target,this);
    if( distMinNode >= distMaxCurrent)
    {
        return psIn;
    }
    else if (node->isLeaf)
    {
        psOut = psIn;
        std::priority_queue<pair<Point<ndim>, double>, vector<pair<Point<ndim>, double>>, DistanceComparator> pq(psOut.begin(),psOut.end());
        for (auto& point : node->data) {
            double distance = point.dist(&target);
            if (distance < distMaxCurrent) {
                if (pq.size() < k) {
                    pq.push({point, distance});
                } else if (distance < pq.top().second) {
                    pq.pop();
                    pq.push({point, distance});
                    distMaxCurrent = calculateDistMaxCurrent(k, target, psOut, pq.top().second);
                }
            }
        }

        vector<std::pair<Point<ndim>,double>> temp;
        while (!pq.empty()) {
            temp.push_back(pq.top());
            pq.pop();
        }

        psOut = temp;
    }
    else if (!node->isLeaf)
    {
        Ball_Tree_Node<ndim>* node1, * node2;
        if (node->leftChild->center->dist(&target) > node->rightChild->center->dist(&target)) {
            node1 = node->rightChild;
            node2 = node->leftChild;
        } else {
            node1 = node->leftChild;
            node2 = node->rightChild;
        }

        vector<std::pair<Point<ndim>,double>> temp = KNN(target, psIn, node1, k);
        psOut = KNN(target, temp, node2, k);
    }
    return psOut;
}

template<int ndim>
void Ball_Tree_Node<ndim>::coverData2() {
    /*
     * Idea:
     * Obtener los puntos min y max de cada dimensión.
     * Entre estos puntos, decidir el más lejano del centroide
     * Formar el radio en base a esta diferencia
     * Multiplicar el radio por raíz de 2 para evitar puntos fuera del círculo
     * */
    if (data.empty()) return;
    Point<ndim>* media = new Point<ndim>; //Buscar punto medio del círculo
    vector<Point<ndim>> min_points(ndim); //Apuntar al punto min,max en cada dimensión
    vector<Point<ndim>> mx_points(ndim);
    double maxDist = 0;
    //Busqueda lineal de la mediaNA, min, max
    for (int d = 0; d < ndim; d++)
    {
        min_points[d] = data[0];
        for (auto x : data)
        {
            if (x.coord[d] > mx_points[d].coord[d])
                mx_points[d] = x;
            if (x.coord[d] < min_points[d].coord[d]) //Hallamos el min y max en cada dimension.
                min_points[d] = x;
        }
        media->coord[d] = (mx_points[d].coord[d] + min_points[d].coord[d])/2;
    }
    for(auto x : data){
        auto temp = x.dist(media);
        if(temp > maxDist)
            maxDist = temp;
    }
    radius = maxDist;
    this->center = media;
    //writeExtremePoints(mx_points,min_points,*center);
}
template<int ndim>
void Ball_Tree_Node<ndim>::build(vector<Point<ndim>> & data_t) {
    saveData(data_t);
    coverData2();
    if(canDivide())
    {
        splitData(); //build sons
    }
    else
    {
        isLeaf = true;
        return;
    }
}

template<int ndim>
Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> Ball_Tree_Node<ndim>::stdToEigen(vector<Point<ndim>>& v) {
    int rows = v.size();
    int cols = ndim;
    Eigen::MatrixXd result(rows,cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result(i,j) = v[i].coord[j];
    return result;
}
template<int ndim>
void Ball_Tree_Node<ndim>::saveData(vector<Point<ndim>> &data_t) {
    data = std::move(data_t);
    currentPoints = data.size();
}
template<int ndim>
void Ball_Tree_Node<ndim>::splitData() {
    /*
     * Idea:
     * Convertir el vector de datos a una matriz de eigen
     * Reducir la matriz a 1 dimensión usando PCA
     * Ordenar conjuntamente la matriz de 1 dimensión y los datos originales (sort_vectors2)
     * Al ordenar la matriz de una dimensión y dividir a la mitad, se está dividiendo perpendicularmente a la orientación de los puntos.
     * Dividir en 2 hijos.
     * */
    pca<double> pca1;

    Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> input = stdToEigen(data);

    pca1.set_input(input);
    pca1.compute_1d();
    Eigen::Matrix<double,Eigen::Dynamic,1> data_1d = pca1.reprojection();

    sort_vectors2(data_1d,data);

    // Hallar punto medio
    std::size_t  const half_size = data.size() / 2;
    vector<Point<ndim>> lowerData(data.begin(), data.begin() + half_size);
    vector<Point<ndim>> higherData(data.begin() + half_size, data.end());
    data.clear(); //Liberar datos del padre

    leftChild = new Ball_Tree_Node<ndim>;
    rightChild = new Ball_Tree_Node<ndim>;
    leftChild->parent = this;
    rightChild->parent = this;
    leftChild->isRoot = false;
    rightChild->isRoot = false;
    leftChild->build(lowerData);
    rightChild->build(higherData);
}
template<int ndim>
void Ball_Tree_Node<ndim>::coverData() {
    if (data.empty()) return;
    Point<ndim>* media = new Point<ndim>; //Buscar punto medio del círculo
    vector<Point<ndim>> min_points(ndim); //Apuntar al punto min,max en cada dimensión
    vector<Point<ndim>> mx_points(ndim);
    //Busqueda lineal de la mediaNA, min, max
    for (int d = 0; d < ndim; d++)
    {
        min_points[d] = data[0];
        //sum = 0;
        for (auto x : data)
        {
            //sum += x.coord[d]; // Para encontrar el promedio en cada dimensión.
            if (x.coord[d] > mx_points[d].coord[d])
               mx_points[d] = x;
            if (x.coord[d] < min_points[d].coord[d]) //Hallamos el min y max en cada dimension.
                min_points[d] = x;
        }
        media->coord[d] = (mx_points[d].coord[d] + min_points[d].coord[d])/2;
    }
    //Agrandar círculo
    for (int i = 0; i < ndim; i++)//Comparamos radius con las distancias del medio a los min y max.
    {
        double dist = min_points[i].dist(media);
        if (dist > this->radius)
            radius = dist;
        double dist2 = mx_points[i].dist(media);
        if (dist2 > this->radius)
            radius = dist2;
    }
    this->center = media;
    this->radius = radius * sqrt(2);
    writeExtremePoints(mx_points,min_points,*center);
}

template<int ndim>
bool Ball_Tree_Node<ndim>::canDivide() {
    return (currentPoints > maxPoints);
}
template<int ndim>
void Ball_Tree_Node<ndim>::sort_vectors2(Eigen::Matrix<double, Eigen::Dynamic, 1> &v1, vector<Point<ndim>> &v2) {
    vector<std::pair<double,int>> data_pair;
    for (int i = 0; i < v1.size(); ++i) {
        data_pair.emplace_back(v1(i),i);                          //O(n)
    }
    std::sort(std::begin(data_pair), std::end(data_pair),
              std::greater<std::pair<double,int>>());                   //O(nlogn)

    vector<Point<ndim>> sorted_v2(v2.size());
    for (int i = 0; i < v1.size(); ++i)//O(n*m)
    {
        sorted_v2[i].name = v2[data_pair[i].second].name;
        for (int j = 0; j < ndim; ++j)
        {
            sorted_v2[i].coord[j] = v2[data_pair[i].second].coord[j];
        }
    }
    v2 = sorted_v2;                                                           //O(n*m)
}

template<int ndim>
void Ball_Tree_Node<ndim>::sort_vectors(vector<Point<ndim>> &v1, vector<Point<ndim>> &v2) {
    vector<std::pair<double,int>> data_pair; //valor, índice
    for (int i = 0; i < v1.size(); ++i) {
        data_pair.emplace_back(v1[i].coord[0],i);
    }
    std::sort(std::begin(data_pair), std::end(data_pair), std::greater<std::pair<double,int>>());
    vector<Point<ndim>> sorted_v1(v1.size());
    vector<Point<ndim>> sorted_v2(v2.size());
    for (int i = 0; i < v1.size(); ++i) {
        sorted_v1[i].coord[0] = data_pair[i].first;
        for (int j = 0; j < ndim; ++j)
            sorted_v2[i].coord[j] = v2[data_pair[i].second].coord[j];
    }
    v1 = sorted_v1;
    v2 = sorted_v2;
}

template<int ndim>
void Ball_Tree_Node<ndim>::writeExtremePoints(vector<Point<ndim>> maxp, vector<Point<ndim>> minp, Point<ndim> cen ) {
    std::ofstream outputfile("../textfiles/e_points.txt");
    if(!outputfile.is_open()){
        cout<<"Error al abrir el archivo.\n";
        return;
    }
    std::ostringstream oss;
    for (auto x : maxp){
        for(int c = 0; c < ndim; c++)
            oss<<x.coord[c]<<" ";
        oss << "\n";
    }
    for (auto x : minp){
        for(int c = 0; c < ndim; c++)
            oss<<x.coord[c]<<" ";
        oss << "\n";
    }
    for(int c = 0; c < ndim; c++)
        oss<<cen.coord[c]<<" ";
    outputfile<<oss.str();
    outputfile.close();
}
template<int ndim>
void Ball_Tree_Node<ndim>::writeDataToFile(const std::string& file) {
    std::ofstream outputfile(file);
    if(!outputfile.is_open()){
        cout<<"Error al abrir el archivo.\n";
        return;
    }
    std::ostringstream  oss;
    for(auto d : data)
    {
        for (int i = 0; i < ndim; ++i) {
            oss << d.coord[i] << " ";
        }
        oss << "\n";
    }
    outputfile<<oss.str();
    outputfile.close();
}
template<int ndim>
void Ball_Tree_Node<ndim>::writeCircleToFile(const std::string& file) {
    std::ofstream outputfile(file, std::ios::app);
    if(!outputfile.is_open()){
        cout<<"Error al abrir el archivo.\n";
        return;
    }
    std::ostringstream oss;
    for (int i = 0; i < ndim; ++i) {
        oss<<center->coord[i]<<" ";
    }
    oss << radius << '\n';
    outputfile<<oss.str();
    outputfile.close();
}


#endif //BALL_TREE_CLION_BALL_TREE_NODE_H