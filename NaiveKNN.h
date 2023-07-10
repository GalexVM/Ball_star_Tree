//
// Created by USER on 8/07/2023.
//

#ifndef BALL_TREE_CLION_NAIVEKNN_H
#define BALL_TREE_CLION_NAIVEKNN_H
#include "Point.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <sstream>

using std::pair;

template<int ndim>
class NaiveKNN {
public:
    vector<Point<ndim>> knn;
    vector<std::pair<Point<ndim>,double>> psOut;
    void calculate(int k, Point<ndim>& target, vector<Point<ndim>>& data);
    void printResults();
    void printDistances(Point<ndim> &target, vector<Point<ndim>>& data);
private:
    struct DistanceComparator {
        bool operator()(const pair<Point<ndim>, double>& p1, const pair<Point<ndim>, double>& p2) {
            return p1.second < p2.second;
        }
    };

};

template<int ndim>
void NaiveKNN<ndim>::printDistances(Point<ndim> &target, vector<Point<ndim>>& data) {
    std::ofstream outputfile("../textfiles/distances.txt");
    if(!outputfile.is_open()){
        cout<<"Error al abrir el archivo.\n";
        return;
    }
    std::ostringstream  oss;
    for(auto i : data)
    {
        oss<< i.name <<", "<<i.coord[0]<<", "<<i.coord[1]<<", "<<i.coord[2]<<". "<<target.dist(&i)<<"\n";
    }
    outputfile<<oss.str();
    outputfile.close();
}

template<int ndim>
void NaiveKNN<ndim>::printResults() {
    for(auto x : knn)
    {
        std::cout << x.name<<" ";
        /*for(auto y : x.coord){
            std::cout<<y<<" ";
        }*/
        std::cout<<std::endl;
    }
    /*for(auto i : psOut)
    {
        cout << i.first.name << ", dist: "<< i.second<<endl;
    }*/
}

template<int ndim>
void NaiveKNN<ndim>::calculate(int k, Point<ndim> &target, vector<Point<ndim>>& data) {
    std::priority_queue<pair<Point<ndim>, double>, vector<pair<Point<ndim>, double>>, DistanceComparator> pq;

    for ( auto& point : data) {
        double distance = point.dist(&target);
        if (pq.size() < k) {
            pq.push({point, distance});
        } else if (distance < pq.top().second) {
            pq.pop();
            pq.push({point, distance});
        }
    }

    knn.reserve(k);
    while (!pq.empty()) {
        knn.push_back(pq.top().first);
        pq.pop();
    }

    reverse(knn.begin(), knn.end());  // Si se necesita el orden ascendente de los k vecinos más cercanos

}


#endif //BALL_TREE_CLION_NAIVEKNN_H
