#ifndef SEARCH_H_
#define SEARCH_H_

#include <iostream>
#include <cstdlib>
#include <vector>

class Search {
    private:
        struct Feature {
            int feature_number;
            float feature_accuracy;

            Feature(int number) : feature_number(number) {}
        };

    public:
        // Constructor creates arrays based on passed in int.
        // feature_set_ and result_set_ are double pointers to dynamically
        // allocate array size.
        Search(int number_of_features) : total_number_of_features_(number_of_features),
                                         subset_accuracy(0) {
            feature_set_ = new Feature*[number_of_features];
            result_set_ = new Feature*[number_of_features];

            // Using Feature struct constructor, feature_number
            // is the same as their index in feature_set_.
            for (int i = 0; i < number_of_features; ++i) {
                feature_set_[i] = new Feature(i);
                result_set_[i] = nullptr;
            }
        }

        void forward_selection();
        void backward_eliminiation();
    private:
        int total_number_of_features_;
        float subset_accuracy;

        Feature** feature_set_;
        Feature** result_set_;
};


#endif  // SEARCH_H_