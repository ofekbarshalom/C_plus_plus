#ifndef ATOM_WAREHOUSE_HPP
#define ATOM_WAREHOUSE_HPP

#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <sys/types.h> 
#include <sys/un.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace std;

namespace warehouse {

struct SharedInventory {
    int carbon;
    int hydrogen;
    int oxygen;
};

class AtomWarehouse {
    public:
       	AtomWarehouse();    // Constructor initializes atom counts to zero
		AtomWarehouse(const string& saveFile);	// Constructor initializes from a save file
		~AtomWarehouse();
		
    	void addAtom(const string& type, unsigned int count);    // Add atoms of a given type
   	    bool isValidAtom(const string& type) const;    // Check if atom type is valid
    	void printInventory() const;    // Print the current atom inventory
		bool canDeliver(const string& molecule, unsigned int count) const;   // Check if the warehouse can deliver a given molecule in the specified count
		void deliverMolecule(const string& molecule, int count);   // Deliver a molecule, updating the inventory accordingly
		void handleConsoleInput();
		void handleUDPRequest(const string& input, int udp_fd, struct sockaddr_in& cli_addr, socklen_t cli_len);
		void handleTCPClientMessage(int clientFd, const string& input);
		void setupUnixStreamSocket(const string& path);
		void setupUnixDatagramSocket(const string& path);
		void handleUnixDatagramRequest(const string& input, int sockfd, struct ::sockaddr_un& cli_addr, socklen_t cli_len);
   		void runCombinedServer(int tcpPort, int udpPort); // Run a combined TCP and UDP server to handle requests
		void setTimeout(int t) { timeout = t; }
		void initializeSharedInventory(int carbon, int hydrogen, int oxygen);

    private:
        unordered_map<string, unsigned int> inventory;
		static const unordered_map<string, unordered_map<string, int>> moleculeRequirements;
		int timeout = -1; // Timeout for the server, set to -1 for no timeout
		int unixStreamFd = -1;
		int unixDatagramFd = -1;
		int saveFileFd = -1;
		string streamPath, datagramPath,saveFilePath;
		SharedInventory* sharedInventory = nullptr; // Pointer to shared memory for inventory
};

} // namespace warehouse

#endif // ATOM_WAREHOUSE_HPP
