#ifndef ATOM_WAREHOUSE_HPP
#define ATOM_WAREHOUSE_HPP

#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <sys/types.h> 

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
		void handleConsoleInput();
		void handleUDPRequest(const string& input, int udp_fd, struct sockaddr_in& cli_addr, socklen_t cli_len);
		void handleTCPClientMessage(int clientFd, const string& input);
   		void runCombinedServer(int tcpPort, int udpPort); // Run a combined TCP and UDP server to handle requests
		void setTimeout(int t) { timeout = t; }


    private:
        unordered_map<string, unsigned int> inventory;
		static const unordered_map<string, unordered_map<string, int>> moleculeRequirements;
		int timeout = -1; // Timeout for the server, set to -1 for no timeout
};

} // namespace warehouse

#endif // ATOM_WAREHOUSE_HPP
