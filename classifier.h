
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

class Classifier {
    public:
        friend class Search;

        void read_file(const std::string &);

        // Adding and removing feature of the Classifier class is
        // not be confused with forward search and backward elimination
        // of the Search class.
        // We add features to the feature subset in Classifier to get that
        // feature's or features' accuracy, then removing that feature 
        // from the subset but passing in the accuracy to the Search class.
        // Search's class feature subset uses the feature with the best accuracy
        // as part of the subset for the next best feature accuracy search. 
        void add_feature(const int &);

        void run_NN_algorithm();

    private:
        std::string file_name_;

        float** data_;
        float** normalized_data_;

        int* feature_subset_indices_;
        float* predicted_labels_;

        int number_of_rows_;
        int number_of_columns_;
        int feature_subset_size_;

        float min_;
        float max_;
        float accuracy_;
};

#endif  // CLASSIFIER_H_