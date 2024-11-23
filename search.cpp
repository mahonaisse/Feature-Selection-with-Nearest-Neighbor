#include "search.h"

void Search::forward_selection() {
    int remaining_features = total_number_of_features_;
    bool decreased_accuracy_not_found = true;
    Feature* best_feature = nullptr;

    srand(time(0));

    std::cout << "Beginning search." << '\n' << '\n';

    while (remaining_features > 0 && decreased_accuracy_not_found == true) {
        // Iterate through features.
        for (int i = 0; i < total_number_of_features_; ++i) {
            if (feature_set_[i] != nullptr) {
                // Assign random floating value between 0 and 100 as feature accuracy.
                feature_set_[i]->feature_accuracy = (rand() / (float)RAND_MAX) * 100;

                // Print current result set with iterated feature.
                std::cout << '\t' << "Using feature(s) {";
                // Iterate through result set.
                for (int j = 0; j < total_number_of_features_ - remaining_features; ++j) {
                    std::cout << result_set_[j]->feature_number << ", ";
                }
                std::cout << i << "} with accuracy " << feature_set_[i]->feature_accuracy << '\n'; 

                // Check for best accuracy of current iteration.
                // If there is no current best accuracy, set the first feature
                // as the best.
                if (best_feature == nullptr) {
                    best_feature = feature_set_[i];
                }
                else if (best_feature->feature_accuracy < feature_set_[i]->feature_accuracy) {
                    best_feature = feature_set_[i];
                }
            }
        }   // End of for loop.

        // Print feature set and new feature with best accuracy.
        std::cout << '\n' << "Feature set {";
        for (int j = 0; j < total_number_of_features_ - remaining_features; ++j) {
            std::cout << result_set_[j]->feature_number << ", ";
        }
        std::cout << best_feature->feature_number << "} was best, with an accuracy of " << best_feature->feature_accuracy << "%" << '\n' << '\n';

        // Check for increasing accuracy of subset.
        // If accuracy increased from the new best feature, add the feature to the subset and update
        // the subset's accuracy.
        if (subset_accuracy < best_feature->feature_accuracy) {
            // Remove feature from feature set.
            feature_set_[best_feature->feature_number] = nullptr;

            // Add feature to feature subset.
            result_set_[total_number_of_features_ - remaining_features] = best_feature;

            // Update subset accuracy.
            subset_accuracy = best_feature->feature_accuracy;

            // Reset feature pointer for next iteration.
            best_feature = nullptr;

            --remaining_features;
        }
        // Otherwise, accuracy decreased from the new best feature, so the search ends.
        else {
            std::cout << "(Warning: The accuracy has decreased!)" << '\n';
            std::cout << "Search finished! The best feature subset is {";
            for (int j = 0; j < total_number_of_features_ - remaining_features; ++j) {
                std::cout << result_set_[j]->feature_number;

                if (j + 1 < total_number_of_features_ - remaining_features) {
                    std::cout << ", ";
                }
            }
            std::cout << "} with an accuracy of " << subset_accuracy << "%" << '\n';

            // This ends the while loop.
            decreased_accuracy_not_found = false;
        }

    }   // End of while loop.
};

void Search::backward_eliminiation() {
    int remaining_features = total_number_of_features_;
    bool decreased_accuracy_not_found = true;
    bool print_first = true;
    Feature* best_feature = nullptr;

    // Set result set to feature set.
    // for (int i = 0; i < total_number_of_features_; ++i) {
        // result_set_[i] = feature_set_[i];
    // }
    result_set_ = feature_set_;

    srand(time(0));

    std::cout << "Beginning search." << '\n' << '\n';

    while (remaining_features > 0 && decreased_accuracy_not_found == true) {
        // Iterate through features to remove.
        for (int i = 0; i < total_number_of_features_; ++i) {
            if (result_set_[i] != nullptr) {
                // Assign random floating value between 0 and 100 as feature accuracy.
                result_set_[i]->feature_accuracy = (rand() / (float)RAND_MAX) * 100;

                // Print potential result set with eliminated feature.
                std::cout << '\t' << "Eliminating feature {" << result_set_[i]->feature_number << "}. New feature set {";
                // Iterate through result set, not printing the feature to potentially eliminate.
                print_first = true;
                for (int j = 0; j < total_number_of_features_; ++j) {
                    if (i != j && result_set_[j] != nullptr) {
                        if (print_first == false) {
                            std::cout << ", ";
                        }
                        std::cout << result_set_[j]->feature_number;
                        print_first = false;
                    }
                }
                std::cout << "} with accuracy " << feature_set_[i]->feature_accuracy << '\n'; 

                // Check for best accuracy of current iteration.
                // If there is no current best accuracy, set the first feature
                // as the best.
                if (best_feature == nullptr) {
                    best_feature = feature_set_[i];
                }
                else if (best_feature->feature_accuracy < feature_set_[i]->feature_accuracy) {
                    best_feature = feature_set_[i];
                }
            }
        }   // End of for loop.

        // Print feature set and new feature with best accuracy.
        print_first = true;
        std::cout << '\n' << "Eliminating feature {" << best_feature->feature_number << "} was best. New feature set {";
        for (int i = 0; i < total_number_of_features_; ++i) {
            if (best_feature->feature_number != i && result_set_[i] != nullptr) {
                if (print_first == false) {
                    std::cout << ", ";
                }

                std::cout << result_set_[i]->feature_number;
                print_first = false;
            }
        }
        std::cout << "} with an accuracy of " << best_feature->feature_accuracy << "%" << '\n' << '\n';

        // Check for increasing accuracy of subset.
        // If accuracy increased from the new best feature, add the feature to the subset and update
        // the subset's accuracy.
        if (subset_accuracy < best_feature->feature_accuracy) {
            // Remove feature from result set.
            result_set_[best_feature->feature_number] = nullptr;

            // Update subset accuracy.
            subset_accuracy = best_feature->feature_accuracy;

            // Reset feature pointer for next iteration.
            best_feature = nullptr;

            --remaining_features;
        }
        // Otherwise, accuracy decreased from the new best feature, so the search ends.
        else {
            std::cout << "(Warning: The accuracy has decreased!)" << '\n';
            // Setting this to 0 will print out the final subset and end the loop.
            remaining_features = 0;
        }
        if (remaining_features == 0) {
            std::cout << "Search finished! The best feature subset is {";
            print_first = true;
            for (int i = 0; i < total_number_of_features_; ++i) {
                if (result_set_[i] != nullptr) {
                    if (print_first == false) {
                        std::cout << ", ";
                    }
                    std::cout << result_set_[i]->feature_number;
                    print_first = false;
                }
            }
            std::cout << "} with an accuracy of " << subset_accuracy << "%" << '\n';

            // This ends the while loop.
            decreased_accuracy_not_found = false;
        }

    }   // End of while loop.
};