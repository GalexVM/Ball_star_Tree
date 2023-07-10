cmake_minimum_required(VERSION 3.25)
project(ball_tree_clion)

set(CMAKE_CXX_STANDARD 17)
find_package(Eigen3)
add_executable(ball_tree_clion main.cpp Ball_Tree.h Ball_Tree_Node.h pca.h Point.h NaiveKNN.h)
target_link_libraries(ball_tree_clion Eigen3::Eigen)