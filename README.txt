Project 3: Package Delivery Route Search

Team Members: Tooba Shakeel and Averi Giles

DESCRIPTION
This program uses a graph to represent distribution centers and the routes connecting them. It uses an adjacency list and Breadth-First Search (BFS) to find the shortest routes between centers based on the number of connections.

FEATURES
1. Finds the shortest route between two centers and checks if it is under a given connection limit.
2. Finds a route that visits two required centers before reaching the destination.
3. Finds a round trip that visits every reachable center and returns to the starting center.
4. Finds the best transfer center based on the shortest routes from three starting centers.

FILES
main.cpp - Contains the menu and main program.
graph.h - Contains the graph class.
graph.cpp - Contains the graph functions and route operations.
data/ - Contains the input files.
tests/ - Contains test files for the four operations.

REQUIREMENTS
- C++11 or newer compiler.
- No extra libraries are needed.

HOW TO COMPILE
Open the Giles_Shakeel_Project3 folder in VS Code.

If your terminal is in the parent folder, enter:

cd .\Giles_Shakeel_Project3

Then compile using:

g++ -std=c++11 -Wall -Wextra -pedantic main.cpp graph.cpp -o route_search

HOW TO RUN

Windows:
.\route_search.exe

macOS/Linux:
./route_search

The program uses data/package_routes.txt by default.

To use a different input file, include its path when running the program.

Example:
.\route_search.exe tests/q1_routes.txt

INPUT FORMAT
Each line in the input file contains two center names separated by a space.

Example:
A B
B C
C D

This means A is connected to B, B is connected to C, and C is connected to D. Connections work in both directions.

Lines starting with # are treated as comments. Duplicate connections are ignored.

TESTING
The following files can be used to test the program:

- tests/q1_routes.txt - Tests Question 1.
- tests/q2_routes.txt - Tests Question 2.
- tests/q3_routes.txt - Tests Question 3.
- tests/q4_routes.txt - Tests Question 4.
- tests/no_route.txt - Tests what happens when no route exists.
- data/package_routes.txt - Main input file.
- data/larger_routes.txt - Larger input file.

More information about the test cases can be found in tests/TEST_CASES.txt.

ALGORITHM AND LIMITATIONS
The program uses BFS to find the shortest routes. Since the graph is unweighted, BFS finds the route with the fewest connections.

Question 2 checks both possible orders for visiting the required centers. Question 4 compares the distances from the three starting centers to find the best transfer center.

Question 3 uses BFS with a bitmask to keep track of visited centers. Since this takes more memory as the number of centers increases, the program only supports up to 20 reachable centers for this operation.
