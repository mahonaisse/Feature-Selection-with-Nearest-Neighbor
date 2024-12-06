#include <iostream>
#include <fstream>
#include <cstdlib>

#include "classifier.h"
#include "search.h"

/*
Group: Mario Bertumen – mbert015 – Session 22
- DatasetID: 211
- Small Dataset Results:
    - Forward: Feature Subset: {5,3}, Acc: 0.92
    - Backward: Feature Subset: {2,4,5,7,10} Acc: 0.83
- Large Dataset Results:
    - Forward: Feature Subset: {27,1}, Acc: 0.955
    - Backward: Feature Subset: {1,2,3,5,8,9,10,11,12,13,14,15,17,18,19,20,22,23,24,25,26,27,28,29,30,31,34,35,36,37,38,39,40}, Acc: 0.739
*/

// Text to copy and paste.
/*
g++ main.cpp classifier.cpp search.cpp -Wall -Werror -o run_search.exe
.\run_search.exe

small-test-dataset.txt
large-test-dataset.txt
titanic clean.txt
*/

int total_memory = 0;

void* operator new(size_t size) {
    total_memory += size;
    return malloc(size);
}

void* operator new[](size_t size) {
    total_memory += size;
    return std::malloc(size);
}

void operator delete(void* pointer, size_t size) {
    total_memory -= size;
    std::free(pointer);
}

void operator delete[](void* pointer, size_t size) {
    total_memory -= size;
    std::free(pointer);
}

int main() {
    std::string file_name;

    int choice = 0;

    std::cout << "Please type in the name of the file to test: ";
    std::getline(std::cin, file_name);

    Search list(file_name);

    while (true) {
        std::cout << '\n' << "--- Menu ---" << '\n'
                    << "0 to exit the program." << '\n'
                    << "1 to run forward selection." << '\n'
                    << "2 to run backward elimination." << '\n';
        std::cout << '\n' << "Enter choice (0-2): ";
        
        std::cin >> choice;
        std::cout << '\n';

        switch (choice) {
            case 0:
                exit(0);
            case 1:
                list.run_forward_selection();
                std:: cout << "Total memory allocated is " << total_memory << " bytes." << '\n';
                break;
            case 2:
                list.run_backward_elimination();
                std:: cout << "Total memory allocated " << total_memory << " bytes." << '\n';
                break;
        }

        // Clear cin for next while loop.
        std::cin.clear();
        std::cin.ignore(256, '\n');

    }   // End of while loop.
}