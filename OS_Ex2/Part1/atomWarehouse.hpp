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
   		void runTCPServer(int port);	// Start the TCP server

    private:
        unordered_map<string, unsigned int> inventory;
};

} // namespace warehouse

#endif // ATOM_WAREHOUSE_HPP
