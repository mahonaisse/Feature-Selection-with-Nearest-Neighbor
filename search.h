#ifndef SEARCH_H_
#define SEARCH_H_

#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>

#include "classifier.h"

class Search {
    public:
        // Constructor creates arrays based on passed in int.
        // feature_set_ and result_set_ are double pointers to dynamically
        // allocate array size.
        Search(const std::string &);

        void run_forward_selection();
        void run_backward_elimination();
    private:
        struct Feature {
            int feature_number;
            float feature_accuracy;

            Feature(int number) : feature_number(number) {}
        };

        Classifier* classifier_;

        int total_number_of_features_;
        float subset_accuracy_;

        Feature** feature_set_;
        Feature** result_set_;
};


#endif  // SEARCH_H_