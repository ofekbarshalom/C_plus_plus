# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Targets
all: atom_warehouse atom_supplier molecule_requester

# Server build
atom_warehouse: atomWarehouse.cpp atomWarehouseMain.cpp
	$(CXX) $(CXXFLAGS) -o atom_warehouse atomWarehouseMain.cpp atomWarehouse.cpp

# TCP Client build
atom_supplier: atomSupplier.cpp atomSupplierMain.cpp
	$(CXX) $(CXXFLAGS) -o atom_supplier atomSupplier.cpp atomSupplierMain.cpp

# UDP Client build
molecule_requester: moleculeRequester.cpp moleculeRequesterMain.cpp
	$(CXX) $(CXXFLAGS) -o molecule_requester moleculeRequester.cpp moleculeRequesterMain.cpp

# Clean target
clean:
	rm -f atom_warehouse atom_supplier molecule_requester
