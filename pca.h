//
// Created by USER on 18/06/2023.
//

#ifndef BALL_TREE_CLION_PCA_H
#define BALL_TREE_CLION_PCA_H
#include <algorithm>
#include <Eigen/Dense>
#include <vector>
#include <cmath>

template<typename Type>
class pca {
public:

    enum class sort_order : uint8_t
    {
        none, ascending, descending
    };

    pca() = default;
    ~pca() = default;

    void set_input(const Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>& _input_matrix)
    {
        input_matrix = _input_matrix;
    }

    void calculatePc1()
    {
        normalizeMatrix();
        calculateEigen();
        sort_eigen_vectors(sort_order::ascending);
        pc1 = eigen_vectors.col(0);
    }

    void compute_1d()
    {
        normalizeMatrix();
        calculateEigen();
        projection_matrix = eigen_min();
    }

    void compute(sort_order order = sort_order::ascending)
    {
        normalizeMatrix();
        calculateEigen();
        if (order != sort_order::none)
            sort_eigen_vectors(order);
        projection_matrix = eigen_vectors.block(0,0,eigen_vectors.rows(),2);
    }


    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> reprojection()
    {
        return normalized_matrix * projection_matrix;
    }


    const Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>& get_input_matrix() const { return input_matrix; }
    const Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>& get_centered_matrix() const { return centered_matrix; }
    const Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>& get_normalized_matrix() const { return normalized_matrix; }
    const Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>& get_covariance_matrix() const { return covariance_matrix; }
    const Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>& get_projection_matrix() const { return projection_matrix; }
    const Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>& get_mean() const { return mean_matrix; }
    const Eigen::Matrix<Type, 1, Eigen::Dynamic>& get_eigen_values() const { return eigen_values; };
    const Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>& get_eigen_vectors() const { return eigen_vectors; }
    const Eigen::VectorX<Type>& get_pc1() const {return pc1;}

private:

    // Private Methods

    Eigen::Matrix<Type,Eigen::Dynamic, Eigen::Dynamic> eigen_min()
    {
        int maxIndex = 0;
        for (int i = 0; i < eigen_values.size(); ++i) {
            if (eigen_values(i) > eigen_values(maxIndex))
                maxIndex = i;
        }
        return eigen_vectors.block(0,maxIndex,eigen_vectors.rows(),1);
    }


    void sort_eigen_vectors(sort_order order = sort_order::ascending)
    {
        // Stuff below is done to sort eigen values. This can be done in other ways too.
        std::vector<std::pair<int, int>> eigen_value_index_vector;
        for (int i = 0; i < eigen_values.size(); ++i)
        {
            eigen_value_index_vector.push_back(std::make_pair(eigen_values[i], i));
        }

        if (order == sort_order::ascending)
            std::sort(std::begin(eigen_value_index_vector), std::end(eigen_value_index_vector), std::greater<std::pair<int, int>>());
        else
            std::sort(std::begin(eigen_value_index_vector), std::end(eigen_value_index_vector), std::less<std::pair<int, int>>());

        auto sorted_eigen_values = Eigen::Matrix<Type, 1, Eigen::Dynamic>(eigen_values.cols());
        auto sorted_eigen_vectors = Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>(eigen_vectors.rows(), eigen_vectors.cols());
        for (int i = 0; i < eigen_values.size(); ++i)
        {
            sorted_eigen_values[i] = eigen_values[eigen_value_index_vector[i].second]; //can also be eigen_value_index_vector[i].first
            sorted_eigen_vectors.col(i) = eigen_vectors.col(eigen_value_index_vector[i].second);
        }

        eigen_values = sorted_eigen_values;
        eigen_vectors = sorted_eigen_vectors;
    }

    void normalizeMatrix()
    {
        // Compute a centered version of input matrix
        auto matrix_mean = input_matrix.colwise().mean();
        centered_matrix = input_matrix.rowwise() - matrix_mean;

        /*normalized_matrix = (centered_matrix.transpose()) * centered_matrix;
        covariance_matrix = normalized_matrix / (centered_matrix.rows()-1);*/

        auto columnStd = (input_matrix.array().square().colwise().sum() / input_matrix.rows()).sqrt();
        normalized_matrix = centered_matrix.array().rowwise() / columnStd.array();
        // Compute covariance matrix
        covariance_matrix = (normalized_matrix.rowwise()-matrix_mean).transpose() *
                            (normalized_matrix.rowwise()-matrix_mean) / (Type)(input_matrix.rows() - 1);
    }

    void calculateEigen(sort_order order = sort_order::ascending)
    {
        // Use SelfAdjointEigenSolver to get eigen values and eigen vectors
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>> eigen_solver(covariance_matrix);
        eigen_values = eigen_solver.eigenvalues();
        eigen_vectors = eigen_solver.eigenvectors();
    }

    //
    // Private Attributes
    //

    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> input_matrix;
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> centered_matrix;
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> normalized_matrix;
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> mean_matrix;
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> covariance_matrix;
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> projection_matrix;
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> proyected_pc1;
    Eigen::Matrix<Type, 1, Eigen::Dynamic> eigen_values;
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> eigen_vectors;
    Eigen::VectorX<Type> pc1;
};


#endif //BALL_TREE_CLION_PCA_H
