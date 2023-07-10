//
// Created by USER on 30/06/2023.
//
#ifndef BALL_TREE_CLION_POINT_H
#define BALL_TREE_CLION_POINT_H
#include <utility>
#include<vector>
#include <Eigen/Dense>
#include <string>
using std::vector;
using std::swap;

template<int ndim>
struct Point
{
    vector<double> coord;
    std::string name;
    bool empty = false;
    Point(vector<double> v):coord(std::move(v)){}
    Point(vector<double>v, std::string n):coord(std::move(v)),name(std::move(n)){}
    Point(bool a) { coord = std::vector<double>(ndim, 0); empty=a;}
    Point() { coord = std::vector<double>(ndim, 0);}
    Point(Point<ndim> const &p2):coord(p2.coord),name(p2.name),empty(false){}

    double dist(Point<ndim>* p2);
    double dist(Point<ndim> p2);
};

template<int ndim>
double Point<ndim>::dist(Point<ndim> *p2) {
    double sum = 0;
    for (int i = 0; i < ndim; i++) {
        sum += pow(coord[i] - p2->coord[i], 2);
    }
    return sqrt(sum);
}

template<int ndim>
double Point<ndim>::dist(Point<ndim> p2) {
    double sum = 0;
    for (int i = 0; i < ndim; i++) {
        sum += pow(coord[i] - p2.coord[i], 2);
    }
    return sqrt(sum);
}
#endif //BALL_TREE_CLION_POINT_H
