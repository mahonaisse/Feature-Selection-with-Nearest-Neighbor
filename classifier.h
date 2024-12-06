#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

class Classifier {
    public:
        void read_file(const std::string &);
        void add_feature(const int &);
        void run_algorithm();

    private:
        std::string file_name_;

        double** data_;

        int* feature_subset_indices_;
        double* predicted_labels_;

        int number_of_rows_;
        int number_of_columns_;
        int feature_subset_size_;

        double accuracy_;
};

#endif  // CLASSIFIER_H_