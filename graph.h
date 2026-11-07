#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <vector>

class Graph {
public:
    bool loadFromFile(const std::string& filename);
    std::size_t vertexCount() const;
    std::size_t edgeCount() const;
    void display() const;

    void question1(const std::string& start,
                   const std::string& destination, int limit) const;
    void question2(const std::string& start, const std::string& destination,
                   const std::string& requiredB, const std::string& requiredC) const;
    void question3(const std::string& start) const;
    void question4(const std::string& originA, const std::string& originB,
                   const std::string& originC) const;

private:
    std::unordered_map<std::string, std::vector<std::string>> adjacency;

    bool hasVertex(const std::string& name) const;
    std::vector<std::string> shortestPath(const std::string& start,
                                          const std::string& destination) const;
    static std::string pathToString(const std::vector<std::string>& path);
};

#endif
