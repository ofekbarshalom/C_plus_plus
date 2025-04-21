// ofekbarshalom@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "Graph.hpp"
#include "Queue.hpp"
#include "UnionFind.hpp"
#include "Algorithms.hpp"

using namespace graph;
using namespace algorithms;

//Graph tests
TEST_CASE("Graph test") {
    Graph g(5); // Create a graph with 5 vertices

    // addEdge tests
    SUBCASE("Add undirected edge") {
        g.addEdge(0, 1, 2);
        CHECK(g.hasEdge(0, 1) == true);
        CHECK(g.hasEdge(1, 0) == true);
    }

    SUBCASE("Edge duplication check") {
        g.addEdge(1, 4, 1);
        g.addEdge(1, 4, 1); // should be ignored
        int found = 0;

        const NeighborList& neighbors = g.getNeighbors(1);
        for (int i = 0; i < neighbors.size; ++i) {
            if (neighbors.neighbors[i].id == 4) {
                found++;
            }
        }

        CHECK(found == 1);
    }

    // addDirectedEdge test
    SUBCASE("Add directed edge") {
        g.addDirectedEdge(0, 1, 2);
        CHECK(g.hasEdge(0, 1) == true);
        CHECK(g.hasEdge(1, 0) == false);
    }

    // removeEdge test
    SUBCASE("Remove edge") {
        g.addEdge(2, 3, 5);
        g.removeEdge(2, 3);
        CHECK(g.hasEdge(2, 3) == false);
        CHECK(g.hasEdge(3, 2) == false);
    }

    // getNumVertices test
    SUBCASE("Get vertex count") {
        CHECK(g.getNumVertices() == 5);
    }

    // hasEdge test
    SUBCASE("hasEdge correctness") {
        g.addEdge(0, 2, 1);
        CHECK(g.hasEdge(0, 2) == true);
        CHECK(g.hasEdge(2, 0) == true);
        CHECK(g.hasEdge(1, 3) == false); // no edge added
    }

    // getNeighbors test
    SUBCASE("getNeighbors returns correct neighbors") {
        g.addEdge(1, 3, 7);
        g.addEdge(1, 4, 5);
        const NeighborList& neighbors = g.getNeighbors(1);
        CHECK(neighbors.size == 2);

        bool found3 = false, found4 = false;
        for (int i = 0; i < neighbors.size; ++i) {
            if (neighbors.neighbors[i].id == 3) found3 = true;
            if (neighbors.neighbors[i].id == 4) found4 = true;
        }

        CHECK(found3 == true);
        CHECK(found4 == true);
    }
}

//Queue tests
TEST_CASE("Queue test") {
    Queue q;

    SUBCASE("empty check") {
        CHECK(q.isEmpty() == true);
        q.enqueue(1);
        CHECK(q.isEmpty() == false);
    }

    SUBCASE("enqueue + dequeue") {
        q.enqueue(1);
        CHECK(q.dequeue() == 1);
    }
}

// Union find tests
TEST_CASE("UnionFind test") {

    SUBCASE("find and connected") {
        UnionFind uf(5);
        CHECK(uf.find(2) == 2);
        CHECK(uf.connected(2, 3) == false);
    }

    SUBCASE("unite and connected") {
        UnionFind uf(5);
        uf.unite(1, 3);
        CHECK(uf.connected(1, 3) == true);
        uf.unite(3, 4);
        CHECK(uf.connected(1, 4) == true);
    }

    SUBCASE("find returns same root after chaining") {
        UnionFind uf(5);
        uf.unite(0, 1);
        uf.unite(1, 2);
        CHECK(uf.find(0) == uf.find(2));
    }
}

// Algorithms tests

TEST_CASE("BFS algorithm") {
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    Graph bfsTree = Algorithms::bfs(g, 0);
    CHECK(bfsTree.getNumVertices() == 5);
    CHECK(bfsTree.hasEdge(0, 1));
    CHECK(bfsTree.hasEdge(0, 2));
}

TEST_CASE("DFS algorithm") {
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    Graph dfsTree = Algorithms::dfs(g, 0);
    CHECK(dfsTree.getNumVertices() == 5);
    CHECK(dfsTree.hasEdge(0, 1));
    CHECK(dfsTree.hasEdge(1, 2));
}

TEST_CASE("Dijkstra algorithm") {
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(0, 2, 10);
    g.addEdge(2, 3, 1);

    Graph dijkstraTree = Algorithms::dijkstra(g, 0);
    CHECK(dijkstraTree.hasEdge(0, 1));
    CHECK(dijkstraTree.hasEdge(1, 2));
    CHECK(dijkstraTree.hasEdge(2, 3));
}

TEST_CASE("Prim algorithm") {
    Graph g(4);
    g.addEdge(0, 1, 2);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 4);

    Graph primTree = Algorithms::prim(g);
    CHECK(primTree.getNumVertices() == 4);
    CHECK(primTree.hasEdge(1, 2)); // edge with weight 1
    CHECK(primTree.hasEdge(0, 1));
}

TEST_CASE("Kruskal algorithm") {
    Graph g(4);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 5);

    Graph kruskalTree = Algorithms::kruskal(g);
    CHECK(kruskalTree.getNumVertices() == 4);
    CHECK(kruskalTree.hasEdge(1, 2));
    CHECK(kruskalTree.hasEdge(1, 3));
    CHECK(kruskalTree.hasEdge(0, 2));
}
