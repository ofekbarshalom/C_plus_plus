#ifndef ATOM_WAREHOUSE_HPP
#define ATOM_WAREHOUSE_HPP

#include <string>
#include <unordered_map>

using namespace std;

namespace warehouse {

class AtomWarehouse {
    public:
       	AtomWarehouse();    // Constructor initializes atom counts to zero
    	void addAtom(const string& type, unsigned int count);    // Add atoms of a given type
   	    bool isValidAtom(const string& type) const;    // Check if atom type is valid
    	void printInventory() const;    // Print the current atom inventory
		bool canDeliver(const string& molecule, unsigned int count) const;   // Check if the warehouse can deliver a given molecule in the specified count
		void deliverMolecule(const string& molecule, int count);   // Deliver a molecule, updating the inventory accordingly
   		void runCombinedServer(int tcpPort, int udpPort); // Run a combined TCP and UDP server to handle requests

    private:
        unordered_map<string, unsigned int> inventory;
		static const unordered_map<string, unordered_map<string, int>> moleculeRequirements;
};

} // namespace warehouse

#endif // ATOM_WAREHOUSE_HPP
