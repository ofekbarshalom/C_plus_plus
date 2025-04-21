// ofekbarshalom@gmail.com

#include "UnionFind.hpp"

// Constructor
UnionFind::UnionFind(int size) {
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }
}

// Destructor
UnionFind::~UnionFind() {
    // No dynamic memory to release
}

// Find with path compression
int UnionFind::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

// Union by rank
void UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);

    if (rootX == rootY) return;

    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

// Check if x and y are in the same set
bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}
