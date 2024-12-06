#include "classifier.h"

// For some reason, the class constructor does not work with any of the code
// in this function, both directly in the constructor or even calling this
// function in the constructor.
// So we must call this function after creating the class in main.
void Classifier::read_file(const std::string & file_name) {
    // Set member variables here.
    file_name_ = file_name;
    number_of_rows_ = 0;
    number_of_columns_ = 0;
    feature_subset_size_ = 0;
    accuracy_ = 0;

    std::ifstream file;

    file.open(file_name);

    if (!file.is_open()) {
        std::cout << "Could not open file " << file_name_ << '\n';
        return;
    }
    std::cout << "Opened file " << file_name_ << "." << '\n';

    std::string line;
    std::string word;

    double element;
    int row_iter = 0;
    int column_iter = 0;
    
    // Get number of columns by reading the first line word by word.
    std::getline(file, line);
    ++number_of_rows_;
    std::stringstream s(line);

    while (s >> word) {
        ++number_of_columns_;
    }

    // Get number of rows by reading each line one by one.
    while (std::getline(file, line)) {
        ++number_of_rows_;
    }

    // Create arrays of doubles based on sizes from iterated data.
    // data_[rows Y][columns X]
    data_ = new double*[number_of_rows_];
    
    for (int i = 0; i < number_of_rows_; ++i) {
        data_[i] = new double[number_of_columns_];
    }

    feature_subset_indices_ = new int[number_of_columns_];
    predicted_labels_ = new double[number_of_rows_];

    // Return to start of reading file.
    file.clear();
    file.seekg(0);

    // Read in dataset into 2D array.
    while (std::getline(file, line)) {
        std::stringstream s(line);

        column_iter = 0;

        while (s >> word) {
            // Convert current string word into a double element.
            element = std::stod(word);

            data_[row_iter][column_iter] = element;
            ++column_iter;
        }
        ++row_iter;
    }

    file.close();
};

void Classifier::add_feature(const int & instance_number) {
    bool valid_instance = true;
    bool print_first = true;

    // Check if number falls within bounds of the dataset 1 to number of columns - 1.
    // The bounds begin at 1, because index 0 of the dataset are the features' labels.
    if (instance_number < 1 || number_of_columns_ - 1 < instance_number) {
        std::cout << "Invalid feature number to add to feature subset." << '\n';
        valid_instance = false;
    }

    // Check if number has not been input already.
    for (int i = 0; i < feature_subset_size_; ++i) {
        if (feature_subset_indices_[i] == instance_number) {
            std::cout << "Feature number is already apart of the feature subset." << '\n';
            valid_instance = false;
        }
    }

    // Add number to subset and increase size if number is valid from previous checks.
    if (valid_instance == true) {
        feature_subset_indices_[feature_subset_size_] = instance_number;
        ++feature_subset_size_;
    }
    
    // Output feature subset. Do not output if the subset is empty.
    if (feature_subset_size_ != 0) {
        std::cout << "The feature subset consists of: {";
        for (int i = 0; i < feature_subset_size_; ++i) {
            if (print_first == false) {
                std::cout << ", ";
            }

            std::cout << feature_subset_indices_[i];
            print_first = false;
        }
        std::cout << "}" << '\n';
    }
};

void Classifier::run_algorithm() {
    double distance = 0;
    double closest_distance = 0;
    int closest_feature_index = -1;

    // Use for printing commas while printing an array.
    bool print_first = true;

    double number_of_correct_labels = 0;

    if (feature_subset_size_ == 0) {
        std::cout << "There are no features in the feature subset to run NN algorithm on." << '\n';
        return;
    }

    for (int i = 0; i < number_of_rows_; ++i) {
        // Reset values for next iteration.
        closest_distance = 0;
        closest_feature_index = -1;

        for (int j = 0; j < number_of_rows_; ++j) {
            distance = 0;

            // Use leave-one-out validation method, ignoring instance when iterating through the same
            // current instance from the first for loop.
            if (i != j) {
                // Calculate Euclidean distance between current instance i and iterated instance j.
                // Use only certain features for instances i and j.
                for (int k = 0; k < feature_subset_size_; ++k) {
                    distance += pow(data_[i][feature_subset_indices_[k]] - data_[j][feature_subset_indices_[k]], 2);
                }

                distance = sqrt(distance);

                if (distance < closest_distance || closest_feature_index == -1) {
                    closest_feature_index = j;
                    closest_distance = distance;
                }
            }
        }
        predicted_labels_[i] = data_[closest_feature_index][0];
    }

    // Output feature subset.
    std::cout << "Using a feature subset of {";
    for (int i = 0; i < feature_subset_size_; ++i) {
        if (print_first == false) {
            std::cout << ", ";
        }

        std::cout << feature_subset_indices_[i];
        print_first = false;
    }
    std::cout << "}, we get predicted labels:" << '\n';

    // Output predicted labels.
    for (int i = 0; i < number_of_rows_; ++i) {
        std::cout << predicted_labels_[i] << " ";

        // Add a space every 25 labels for better readability.
        if ((i + 1) % 25 == 0 && i != 0) {
            std::cout << '\n';
        }
    }
    std::cout << '\n';

    // Count the number of correct labels.
    for (int i = 0; i < number_of_rows_; ++i) {
        if (data_[i][0] == predicted_labels_[i]) {
            ++number_of_correct_labels;
        }
    }

    // Output the number and percentage of correct labels.
    accuracy_ = number_of_correct_labels / number_of_rows_;
    std::cout << "There are " << number_of_correct_labels << " out of a total of " << number_of_rows_
              << " instances for an accuracy of " << accuracy_ << "." << '\n';
};