#include "graph.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <cstdint>

bool Graph::loadFromFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) return false;

    adjacency.clear();
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream row(line);
        std::string first, second, extra;
        if (!(row >> first >> second)) continue;
        if (row >> extra) {
            std::cerr << "Skipping invalid line (expected two center names): " << line << '\n';
            continue;
        }
        if (first == second) {
            adjacency[first];
            continue;
        }
        auto& firstNeighbors = adjacency[first];
        auto& secondNeighbors = adjacency[second];
        if (std::find(firstNeighbors.begin(), firstNeighbors.end(), second) == firstNeighbors.end())
            firstNeighbors.push_back(second);
        if (std::find(secondNeighbors.begin(), secondNeighbors.end(), first) == secondNeighbors.end())
            secondNeighbors.push_back(first);
    }

    for (auto& entry : adjacency)
        std::sort(entry.second.begin(), entry.second.end());
    return true;
}

std::size_t Graph::vertexCount() const {
    return adjacency.size();
}

std::size_t Graph::edgeCount() const {
    std::size_t total = 0;
    for (const auto& entry : adjacency) total += entry.second.size();
    return total / 2;
}

bool Graph::hasVertex(const std::string& name) const {
    return adjacency.find(name) != adjacency.end();
}

std::vector<std::string> Graph::shortestPath(const std::string& start,
                                              const std::string& destination) const {
    if (!hasVertex(start) || !hasVertex(destination)) return {};

    std::queue<std::string> pending;
    std::unordered_map<std::string, std::string> parent;
    std::unordered_set<std::string> visited;

    pending.push(start);
    visited.insert(start);

    while (!pending.empty()) {
        std::string current = pending.front();
        pending.pop();
        if (current == destination) break;

        for (const std::string& next : adjacency.at(current)) {
            if (visited.insert(next).second) {
                parent[next] = current;
                pending.push(next);
            }
        }
    }

    if (!visited.count(destination)) return {};

    std::vector<std::string> path;
    for (std::string at = destination; at != start; at = parent.at(at))
        path.push_back(at);
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

std::string Graph::pathToString(const std::vector<std::string>& path) {
    if (path.empty()) return "(no route)";
    std::ostringstream output;
    for (std::size_t i = 0; i < path.size(); ++i) {
        if (i) output << " -> ";
        output << path[i];
    }
    return output.str();
}

void Graph::question1(const std::string& start, const std::string& destination,
                      int limit) const {
    if (limit <= 0) {
        std::cout << "No qualifying route: x must be greater than 0.\n";
        return;
    }
    auto path = shortestPath(start, destination);
    if (path.empty()) {
        std::cout << "No route exists between " << start << " and " << destination << ".\n";
        return;
    }
    int connections = static_cast<int>(path.size()) - 1;
    if (connections < limit) {
        std::cout << "Shortest qualifying route: " << pathToString(path)
                  << "\nConnections: " << connections << '\n';
    } else {
        std::cout << "No route uses fewer than " << limit
                  << " connections. The shortest route needs " << connections << ".\n";
    }
}

void Graph::question2(const std::string& start, const std::string& destination,
                      const std::string& requiredB, const std::string& requiredC) const {
    auto combine = [](const std::vector<std::string>& first,
                      const std::vector<std::string>& second) {
        if (first.empty() || second.empty()) return std::vector<std::string>{};
        std::vector<std::string> result = first;
        result.insert(result.end(), second.begin() + 1, second.end());
        return result;
    };

    auto route1 = combine(combine(shortestPath(start, requiredB),
                                  shortestPath(requiredB, requiredC)),
                          shortestPath(requiredC, destination));
    auto route2 = combine(combine(shortestPath(start, requiredC),
                                  shortestPath(requiredC, requiredB)),
                          shortestPath(requiredB, destination));

    if (route1.empty() && route2.empty()) {
        std::cout << "No route visits both required centers and reaches the destination.\n";
        return;
    }
    const auto& best = (route2.empty() || (!route1.empty() && route1.size() <= route2.size()))
                       ? route1 : route2;
    std::cout << "Shortest route through " << requiredB << " and " << requiredC
              << ": " << pathToString(best)
              << "\nConnections: " << best.size() - 1 << '\n';
}

void Graph::question3(const std::string& start) const {
    if (!hasVertex(start)) {
        std::cout << "Starting center " << start << " is not in the graph.\n";
        return;
    }

    // Find the component reachable from the requested starting center.
    std::vector<std::string> vertices;
    std::queue<std::string> discover;
    std::unordered_set<std::string> seen;
    discover.push(start);
    seen.insert(start);
    while (!discover.empty()) {
        auto current = discover.front();
        discover.pop();
        vertices.push_back(current);
        for (const auto& next : adjacency.at(current)) {
            if (seen.insert(next).second) discover.push(next);
        }
    }

    // Exact BFS on (current vertex, visited-vertices bitmask) finds the
    // shortest walk that covers the component and returns to the start.
    if (vertices.size() > 20) {
        std::cout << "This exact search supports up to 20 reachable centers; this component has "
                  << vertices.size() << ".\n";
        return;
    }

    std::unordered_map<std::string, int> index;
    for (int i = 0; i < static_cast<int>(vertices.size()); ++i) index[vertices[i]] = i;

    struct State {
        int current;
        std::uint32_t mask;
    };
    struct StateHash {
        std::size_t operator()(const std::uint64_t value) const {
            return std::hash<std::uint64_t>{}(value);
        }
    };
    auto encode = [](int current, std::uint32_t mask) -> std::uint64_t {
        return (static_cast<std::uint64_t>(mask) << 5) | static_cast<unsigned>(current);
    };

    const std::uint32_t fullMask = (1u << vertices.size()) - 1u;
    const int startIndex = index.at(start);
    std::queue<State> queue;
    std::unordered_map<std::uint64_t, std::uint64_t> parent;
    std::unordered_set<std::uint64_t, StateHash> visited;

    State initial{startIndex, 1u << startIndex};
    auto initialKey = encode(initial.current, initial.mask);
    queue.push(initial);
    visited.insert(initialKey);

    std::uint64_t goalKey = initialKey;
    bool found = false;
    while (!queue.empty()) {
        State state = queue.front();
        queue.pop();
        auto key = encode(state.current, state.mask);
        if (state.current == startIndex && state.mask == fullMask) {
            goalKey = key;
            found = true;
            break;
        }

        for (const auto& neighbor : adjacency.at(vertices[state.current])) {
            int nextIndex = index.at(neighbor);
            State next{nextIndex, state.mask | (1u << nextIndex)};
            auto nextKey = encode(next.current, next.mask);
            if (visited.insert(nextKey).second) {
                parent[nextKey] = key;
                queue.push(next);
            }
        }
    }

    if (!found) {
        std::cout << "No round trip covering every reachable center exists.\n";
        return;
    }

    std::vector<int> reversedIndices;
    std::uint64_t key = goalKey;
    while (key != initialKey) {
        reversedIndices.push_back(static_cast<int>(key & 31u));
        key = parent.at(key);
    }
    reversedIndices.push_back(startIndex);
    std::reverse(reversedIndices.begin(), reversedIndices.end());

    std::vector<std::string> route;
    for (int i : reversedIndices) route.push_back(vertices[i]);
    std::cout << "Shortest round trip: " << pathToString(route)
              << "\nCenters visited: " << vertices.size()
              << "\nConnections: " << route.size() - 1 << '\n';
}

void Graph::question4(const std::string& originA, const std::string& originB,
                      const std::string& originC) const {
    if (!hasVertex(originA) || !hasVertex(originB) || !hasVertex(originC)) {
        std::cout << "One or more package origins are not in the graph.\n";
        return;
    }

    std::string bestCenter;
    int bestTotal = std::numeric_limits<int>::max();
    for (const auto& entry : adjacency) {
        const std::string& candidate = entry.first;
        if (candidate == originA || candidate == originB || candidate == originC) continue;

        auto pathA = shortestPath(originA, candidate);
        auto pathB = shortestPath(originB, candidate);
        auto pathC = shortestPath(originC, candidate);
        if (pathA.empty() || pathB.empty() || pathC.empty()) continue;

        int total = static_cast<int>(pathA.size() + pathB.size() + pathC.size()) - 3;
        if (total < bestTotal || (total == bestTotal && candidate < bestCenter)) {
            bestTotal = total;
            bestCenter = candidate;
        }
    }

    if (bestCenter.empty()) {
        std::cout << "No different transfer center is reachable from all three origins.\n";
        return;
    }

    std::cout << "Best transfer center: " << bestCenter
              << "\nTotal connections: " << bestTotal
              << "\nRoute from " << originA << ": " << pathToString(shortestPath(originA, bestCenter))
              << "\nRoute from " << originB << ": " << pathToString(shortestPath(originB, bestCenter))
              << "\nRoute from " << originC << ": " << pathToString(shortestPath(originC, bestCenter))
              << '\n';
}

void Graph::display() const {
    std::vector<std::string> names;
    for (const auto& entry : adjacency) names.push_back(entry.first);
    std::sort(names.begin(), names.end());

    std::cout << "Adjacency list (undirected):\n";
    for (const auto& name : names) {
        std::cout << name << ": ";
        const auto& neighbors = adjacency.at(name);
        for (std::size_t i = 0; i < neighbors.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << neighbors[i];
        }
        std::cout << '\n';
    }
}
