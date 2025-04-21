// ofekbarshalom@gmail.com

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

struct UnionFind {
    int parent[100];
    int rank[100];

    UnionFind(int size); // Constructor
    ~UnionFind(); // Destructor

    int find(int x);      // find the root of x with path compression
    void unite(int x, int y); // union two sets by rank
    bool connected(int x, int y); // check if two elements are in the same set
};

#endif //UNIONFIND_HPP
