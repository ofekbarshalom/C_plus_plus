// ofekbarshalom@gmail.com

#include "Algorithms.hpp"
#include "Queue.hpp"
#include "UnionFind.hpp"
#include <climits>

using namespace std;
using namespace graph;

const int INF = INT_MAX;

namespace algorithms {
    // Breadth-First Search (BFS)
    Graph Algorithms::bfs(const Graph& graph, int start) {
        int n = graph.getNumVertices();
        Graph tree(n); // Create a new graph to store the BFS tree
        Queue queue;
        bool visited[MAX_SIZE] = {false}; // Track which vertices we've already visited

        // Start from the root node: mark as visited and enqueue
        queue.enqueue(start);
        visited[start] = true;

        while (!queue.isEmpty()) {
            int current = queue.dequeue();

            // Look at all of its neighbors in the original graph
            const NeighborList& neighbors = graph.getNeighbors(current);

            // Iterate through each neighbor
            for (int i = 0; i < neighbors.size; ++i) {
                int neighborId = neighbors.neighbors[i].id;

                // If the neighbor hasn't been visited yet
                if (!visited[neighborId]) {
                    visited[neighborId] = true; // Mark it as visited
                    queue.enqueue(neighborId);
                    tree.addDirectedEdge(current, neighborId); // Add the undirected edge to the BFS tree
                }
            }
        }
        // Return the final BFS tree
        return tree;
    }

    // Helper function for DFS
    void dfsVisit(const Graph& graph, Graph& tree, bool visited[], int current) {
        visited[current] = true;

        const NeighborList& neighbors = graph.getNeighbors(current);
        for (int i = 0; i < neighbors.size; ++i) {
            int neighborId = neighbors.neighbors[i].id;

            if (!visited[neighborId]) {
                tree.addDirectedEdge(current, neighborId);
                dfsVisit(graph, tree, visited, neighborId);
            }
        }
    }

    // Depth-First Search (DFS)
    Graph Algorithms::dfs(const Graph& graph, int start) {
        int n = graph.getNumVertices();
        Graph tree(n);
        bool visited[MAX_SIZE] = { false };

        // Start DFS from the given start vertex
        dfsVisit(graph, tree, visited, start);

        // Continue DFS for any disconnected components
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                dfsVisit(graph, tree, visited, i);
            }
        }

        return tree;
    }

    // Helper function: relax the edge u → v
    void relax(int u, int v, int w, int dist[], int parent[], bool visited[]) {
        if (!visited[v] && dist[u] != INF && dist[u] + w < dist[v]) {
            dist[v] = dist[u] + w;
            parent[v] = u;
        }
    }

    // Dijkstra's Algorithm for shortest paths
    Graph Algorithms::dijkstra(const Graph& graph, int start) {
        int n = graph.getNumVertices();
        Graph tree(n);

        int dist[MAX_SIZE];
        int parent[MAX_SIZE];
        bool visited[MAX_SIZE] = { false };

        for (int i = 0; i < n; ++i) {
            dist[i] = INF;
            parent[i] = -1;
        }
        dist[start] = 0;

        for (int count = 0; count < n; ++count) {
            int minDist = INF;
            int u = -1;
            for (int i = 0; i < n; ++i) {
                if (!visited[i] && dist[i] < minDist) {
                    minDist = dist[i];
                    u = i;
                }
            }

            if (u == -1) break;
            visited[u] = true;

            const NeighborList& neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.size; ++i) {
                int v = neighbors.neighbors[i].id;
                int w = neighbors.neighbors[i].weight;
                relax(u, v, w, dist, parent, visited);
            }
        }

        // Rebuild shortest-path tree
        for (int v = 0; v < n; ++v) {
            if (parent[v] != -1) {
                int u = parent[v];
                int weight = dist[v] - dist[u];
                tree.addDirectedEdge(u, v, weight);
            }
        }

        return tree;
    }

    // Prim's Algorithm for Minimum Spanning Tree (MST)
    Graph Algorithms::prim(const Graph& graph) {
        int n = graph.getNumVertices();
        Graph mst(n); // Store the Minimum Spanning Tree

        int key[MAX_SIZE];
        int parent[MAX_SIZE];
        bool visited[MAX_SIZE] = { false };

        // Initialize all keys as INF and parents as -1
        for (int i = 0; i < n; ++i) {
            key[i] = INF;
            parent[i] = -1;
        }

        key[0] = 0; // Start from vertex 0

        // Repeat n times to include all vertices
        for (int count = 0; count < n; ++count) {
            // Find the unvisited vertex with the smallest key value
            int u = -1;
            int minKey = INF;
            for (int i = 0; i < n; ++i) {
                if (!visited[i] && key[i] < minKey) {
                    minKey = key[i];
                    u = i;
                }
            }

            if (u == -1) break; // No valid node found

            visited[u] = true; // Add this vertex to MST

            // Update key values of neighbors
            const NeighborList& neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.size; ++i) {
                int v = neighbors.neighbors[i].id;
                int weight = neighbors.neighbors[i].weight;

                if (!visited[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                }
            }
        }

        // Rebuild MST from parent[]
        for (int v = 1; v < n; ++v) {
            if (parent[v] != -1) {
                int u = parent[v];
                int weight = key[v];
                mst.addEdge(u, v, weight); // Undirected edge in MST
            }
        }

        return mst;
    }

//---------------------------------------------------------------------------------

    struct Edge {
        int u, v, weight;
    };

    // Collect all undirected edges from the graph
    int collectEdges(const Graph& graph, Edge edges[]) {
        int count = 0;
        int n = graph.getNumVertices();

        for (int u = 0; u < n; ++u) {
            const NeighborList& neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.size; ++i) {
                int v = neighbors.neighbors[i].id;
                int w = neighbors.neighbors[i].weight;

                if (u < v) { // Avoid duplicates
                    edges[count++] = {u, v, w};
                }
            }
        }
        return count;
    }

    // Sort edges by weight using bubble sort
    void sortEdges(Edge edges[], int edgeCount) {
        for (int i = 0; i < edgeCount - 1; ++i) {
            for (int j = 0; j < edgeCount - i - 1; ++j) {
                if (edges[j].weight > edges[j + 1].weight) {
                    Edge temp = edges[j];
                    edges[j] = edges[j + 1];
                    edges[j + 1] = temp;
                }
            }
        }
    }

    Graph Algorithms::kruskal(const Graph& graph) {
        int n = graph.getNumVertices();
        Graph mst(n);

        Edge edges[MAX_SIZE * MAX_SIZE];
        int edgeCount = collectEdges(graph, edges);
        sortEdges(edges, edgeCount);

        UnionFind uf(n);
        for (int i = 0; i < edgeCount; ++i) {
            int u = edges[i].u;
            int v = edges[i].v;
            int w = edges[i].weight;

            if (!uf.connected(u, v)) {
                uf.unite(u, v);
                mst.addEdge(u, v, w);
            }
        }

        return mst;
    }


}
