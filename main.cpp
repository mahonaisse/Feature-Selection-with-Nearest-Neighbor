#include <iostream>

#include "search.h"

/*
g++ main.cpp search.cpp -Wall -Werror -o run_search.exe
*/

int main() {
    int number_of_features;
    int choice;

    std::cout << "Please input total number of features: ";
    std::cin >> number_of_features;

    while (true) {
        Search feature_list(number_of_features);

        std::cout << '\n' << "--- Menu ---" << '\n'
                      << "0 to exit the program." << '\n'
                      << "1 to perform foward selection." << '\n'
                      << "2 to perform backward eliminiation." << '\n'
                      << '\n' << "Enter choice (0-2): ";
        std::cin >> choice;
        std::cout << '\n';

        switch (choice) {
            case 0:
                exit(0);
            case 1:
                feature_list.forward_selection();
                break;
            case 2:
                feature_list.backward_eliminiation();
                break;
            // case 3:
                // break;
        }

        // Clear cin for next while loop.
        std::cin.clear();
        std::cin.ignore(256, '\n');

    }   // End of while loop.
}