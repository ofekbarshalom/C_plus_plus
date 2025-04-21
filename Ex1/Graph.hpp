// ofekbarshalom@gmail.com

#ifndef GRAPH_HPP
#define GRAPH_HPP

namespace graph {

    // Represents a neighboring vertex
    struct Neighbor {
        int id; // The ID of the neighbor
        int weight; // The weight to get to the neighbor
    };

    // dynamic array of neighbors
    struct NeighborList {
        Neighbor* neighbors; // Dynamic array of Neighbor structs
        int size;
        int capacity;

        NeighborList();
        ~NeighborList();

        void resize();
        void addNeighbor(int id, int weight);
        bool removeNeighbor(int id);
        void print() const;
    };

    class Graph {
        private:
            int numVertices;
            NeighborList* adjacencyList;

        public:
            Graph(int numVertices);
            ~Graph();

            void addEdge(int src, int dest, int weight = 1);
            void addDirectedEdge(int src, int dest, int weight = 1);
            void removeEdge(int src, int dest);
            void print_graph() const;

            int getNumVertices() const;
            bool hasEdge(int src, int dest) const;
            const NeighborList& getNeighbors(int vertex) const;
    };
}

#endif //GRAPH_HPP
