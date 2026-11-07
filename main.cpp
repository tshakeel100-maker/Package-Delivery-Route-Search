#include "graph.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const std::string inputFile = (argc >= 2) ? argv[1] : "data/package_routes.txt";

    Graph graph;
    if (!graph.loadFromFile(inputFile)) {
        std::cerr << "Could not open input file: " << inputFile << '\n';
        std::cerr << "Run the program from the project folder or provide the file path.\n";
        return 1;
    }

    std::cout << "Package Delivery Route Search\n";
    std::cout << "Loaded " << graph.vertexCount() << " centers and "
              << graph.edgeCount() << " undirected routes from " << inputFile << ".\n";

    int choice = 0;
    while (true) {
        std::cout << "\nMenu\n"
                  << "1. Shortest route from A to B with fewer than x connections\n"
                  << "2. Shortest route from A to D through B and C (either order)\n"
                  << "3. Visit every center reachable from A and return to A\n"
                  << "4. Find the best transfer center for packages from A, B, and C\n"
                  << "5. Display graph\n"
                  << "0. Exit\n"
                  << "Choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "\nInvalid input. Exiting.\n";
            break;
        }

        if (choice == 0) {
            std::cout << "Goodbye.\n";
            break;
        }

        if (choice == 1) {
            std::string start, destination;
            int limit;
            std::cout << "Starting center: ";
            std::cin >> start;
            std::cout << "Destination center: ";
            std::cin >> destination;
            std::cout << "Maximum connection limit x (route must use fewer than x): ";
            std::cin >> limit;
            graph.question1(start, destination, limit);
        } else if (choice == 2) {
            std::string start, destination, b, c;
            std::cout << "Starting center: ";
            std::cin >> start;
            std::cout << "Destination center: ";
            std::cin >> destination;
            std::cout << "Required center B: ";
            std::cin >> b;
            std::cout << "Required center C: ";
            std::cin >> c;
            graph.question2(start, destination, b, c);
        } else if (choice == 3) {
            std::string start;
            std::cout << "Starting center: ";
            std::cin >> start;
            graph.question3(start);
        } else if (choice == 4) {
            std::string a, b, c;
            std::cout << "Package origin 1: ";
            std::cin >> a;
            std::cout << "Package origin 2: ";
            std::cin >> b;
            std::cout << "Package origin 3: ";
            std::cin >> c;
            graph.question4(a, b, c);
        } else if (choice == 5) {
            graph.display();
        } else {
            std::cout << "Please enter a menu option from 0 to 5.\n";
        }
    }
    return 0;
}
