#include "search.h"

Search::Search(const std::string &file_name) {
    classifier_ = new Classifier;
    classifier_->read_file(file_name);

    subset_accuracy_ = 0;
    total_number_of_features_ = classifier_->number_of_columns_;

    feature_set_ = new Feature*[total_number_of_features_];
    result_set_ = new Feature*[total_number_of_features_];
}

void Search::run_forward_selection() {
    int remaining_features = total_number_of_features_;
    bool decreased_accuracy_not_found = true;
    Feature* best_feature = nullptr;

    // Reset Classifier class members.
    delete classifier_->feature_subset_indices_;
    classifier_->feature_subset_indices_ = new int[classifier_->number_of_columns_];
    classifier_->feature_subset_size_ = 0;
    classifier_->accuracy_ = 0;

    // Reset Search class members.
    delete result_set_;
    feature_set_ = new Feature*[total_number_of_features_];
    result_set_ = new Feature*[total_number_of_features_];

    // Using Feature struct constructor, feature_number
    // is the same as their index in feature_set_.
    for (int i = 0; i < total_number_of_features_; ++i) {
        feature_set_[i] = new Feature(i);
        result_set_[i] = nullptr;
    }
    subset_accuracy_ = 0;

    srand(time(0));

    std::cout << "Beginning forward search." << '\n' << '\n';
    auto start = std::chrono::high_resolution_clock::now();

    while (remaining_features > 1 && decreased_accuracy_not_found == true) {
        // Iterate through features.
        for (int i = 1; i < total_number_of_features_; ++i) {
            if (feature_set_[i] != nullptr) {
                // Calculate accuracy using NN classifier.
                classifier_->add_feature(i);
                classifier_->run_NN_algorithm();
                --classifier_->feature_subset_size_;

                feature_set_[i]->feature_accuracy = classifier_->accuracy_;

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
        if (subset_accuracy_ < best_feature->feature_accuracy) {
            // Remove feature from feature set.
            feature_set_[best_feature->feature_number] = nullptr;

            // Add feature to feature subsets.
            result_set_[total_number_of_features_ - remaining_features] = best_feature;
            classifier_->add_feature(best_feature->feature_number);

            // Update subset accuracy.
            subset_accuracy_ = best_feature->feature_accuracy;

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
            std::cout << "} with an accuracy of " << subset_accuracy_ << "%" << '\n' << '\n';

            // This ends the while loop.
            decreased_accuracy_not_found = false;
        }

    }   // End of while loop.

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Runtime of " << duration.count() / pow(10, 6) << " seconds." << '\n';

};

void Search::run_backward_elimination() {
    int remaining_features = total_number_of_features_;
    bool decreased_accuracy_not_found = true;
    bool exists_in_subset = false;
    bool print_first = true;
    Feature* best_feature = nullptr;

    // Reset Classifier class members.
    classifier_->feature_subset_size_ = 0;
    classifier_->accuracy_ = 0;

    // Reset Search class members.
    delete result_set_;
    feature_set_ = new Feature*[total_number_of_features_];
    result_set_ = new Feature*[total_number_of_features_];
    
    for (int i = 0; i < total_number_of_features_; ++i) {
        feature_set_[i] = new Feature(i);
        result_set_[i] = nullptr;
    }
    result_set_ = feature_set_;
    subset_accuracy_ = 0;

    srand(time(0));

    std::cout << "Beginning backward eliminiation." << '\n' << '\n';
    auto start = std::chrono::high_resolution_clock::now();

    while (remaining_features > 0 && decreased_accuracy_not_found == true) {
        // Iterate through features to remove.
        for (int i = 1; i < total_number_of_features_; ++i) {
            if (result_set_[i] != nullptr) {
                // Calculate accuracy using NN classifier.
                delete classifier_->feature_subset_indices_;
                classifier_->feature_subset_indices_ = new int[classifier_->number_of_columns_];
                classifier_->feature_subset_size_ = 0;
                
                for (int j = 1; j < total_number_of_features_; ++j) {
                    exists_in_subset = false;
                    for (int k = 0; k < total_number_of_features_; ++k) {
                        if (result_set_[k] != nullptr) {
                            if (j == result_set_[k]->feature_number) {
                                exists_in_subset = true;
                            }
                        }
                    }

                    if (i != j && exists_in_subset == true) {
                        classifier_->add_feature(j);
                    }
                }
                classifier_->run_NN_algorithm();

                result_set_[i]->feature_accuracy = classifier_->accuracy_;

                // Print potential result set with eliminated feature.
                std::cout << '\t' << "Eliminating feature {" << result_set_[i]->feature_number << "}. New feature set {";
                // Iterate through result set, not printing the feature to potentially eliminate.
                print_first = true;
                for (int j = 1; j < total_number_of_features_; ++j) {
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
        for (int i = 1; i < total_number_of_features_; ++i) {
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
        if (subset_accuracy_ < best_feature->feature_accuracy) {
            // Remove feature from result set.
            result_set_[best_feature->feature_number] = nullptr;

            // Update subset accuracy.
            subset_accuracy_ = best_feature->feature_accuracy;

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
            for (int i = 1; i < total_number_of_features_; ++i) {
                if (result_set_[i] != nullptr) {
                    if (print_first == false) {
                        std::cout << ", ";
                    }
                    std::cout << result_set_[i]->feature_number;
                    print_first = false;
                }
            }
            std::cout << "} with an accuracy of " << subset_accuracy_ << "%" << '\n' << '\n';

            // This ends the while loop.
            decreased_accuracy_not_found = false;
        }

    }   // End of while loop.

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Runtime of " << duration.count() / pow(10, 6) << " seconds." << '\n';
};