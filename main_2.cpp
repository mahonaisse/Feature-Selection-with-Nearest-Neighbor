#include <iostream>

#include "classifier.h"

/*
g++ main_2.cpp classifier.cpp -Wall -Werror -o run_classifier.exe
*/

int main() {
    // There are two datasets to use. Comment out the dataset you do not want to use.

    std::string file_name = "small-test-dataset.txt";
    // std::string file_name = "large-test-dataset.txt";

    int choice = 0;
    int last_choice = 0;
    int feature_number;

    Classifier classifier;
    classifier.read_file(file_name);

    while (true) {
        if (last_choice != 1) {
            std::cout << '\n' << "--- Menu ---" << '\n'
                      << "0 to exit the program." << '\n'
                      << "1 to add feature." << '\n'
                      << "2 to run NN algorithm." << '\n';
        }
        std::cout << '\n' << "Enter choice (0-2): ";
        
        std::cin >> choice;
        last_choice = choice;
        std::cout << '\n';

        switch (choice) {
            case 0:
                exit(0);
            case 1:
                std::cout << "Please input which feature to add to subset: ";
                std::cin >> feature_number;

                classifier.add_feature(feature_number);

                break;
            case 2:
                classifier.run_algorithm();

                break;
        }

        // Clear cin for next while loop.
        std::cin.clear();
        std::cin.ignore(256, '\n');

    }   // End of while loop.
}