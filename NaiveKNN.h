//
// Created by USER on 8/07/2023.
//

#ifndef BALL_TREE_CLION_NAIVEKNN_H
#define BALL_TREE_CLION_NAIVEKNN_H
#include "Point.h"
#include <iostream>
#include <algorithm>


template<int ndim>
class NaiveKNN {
public:
    vector<Point<ndim>> data;
    vector<Point<ndim>> r;
    void calculate(int k, Point<ndim>& target, vector<Point<ndim>>& data);
    void printResults();

};

template<int ndim>
void NaiveKNN<ndim>::printResults() {
    for(auto x : r)
    {
        std::cout << x.name<<" ";
        for(auto y : x.coord){
            std::cout<<y<<" ";
        }
        std::cout<<std::endl;
    }
}

template<int ndim>
void NaiveKNN<ndim>::calculate(int k, Point<ndim> &target, vector<Point<ndim>>& data) {

    vector<std::pair<Point<ndim>,double>> psOut;
    std::make_heap(psOut.begin(),psOut.end(),[](std::pair<Point<ndim>,double>& p1, std::pair<Point<ndim>,double>&p2){
        return p1.second < p2.second;
    });
    for(auto a : (data))
    {
        double d = a.dist(&target);

        psOut.push_back(std::make_pair(a,d));
        std::push_heap(psOut.begin(), psOut.end(),[](std::pair<Point<ndim>,double>& p1, std::pair<Point<ndim>,double>& p2){
            return p1.second < p2.second;
        });

        if(psOut.size()>k)
        {
            std::pop_heap(psOut.begin(), psOut.end(), [](std::pair<Point<ndim>,double>& p1, std::pair<Point<ndim>,double>& p2){
                return p1.second < p2.second;
            });
            psOut.pop_back();
        }

    }
    r = vector<Point<ndim>>(psOut.size());
    std::transform(psOut.begin(),psOut.end(),r.begin(),[](const std::pair<Point<ndim>,int> p){
        return p.first;
    });

}


#endif //BALL_TREE_CLION_NAIVEKNN_H
