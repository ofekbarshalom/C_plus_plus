CXX = g++
CXXFLAGS = -Wall -std=c++20

all: drinks_bar atom_supplier molecule_requester

drinks_bar: atomWarehouse.cpp atomWarehouseMain.cpp
	$(CXX) $(CXXFLAGS) -o drinks_bar atomWarehouse.cpp atomWarehouseMain.cpp

atom_supplier: atomSupplier.cpp atomSupplierMain.cpp
	$(CXX) $(CXXFLAGS) -o atom_supplier atomSupplier.cpp atomSupplierMain.cpp

molecule_requester: moleculeRequester.cpp moleculeRequesterMain.cpp
	$(CXX) $(CXXFLAGS) -o molecule_requester moleculeRequester.cpp moleculeRequesterMain.cpp

clean:
	rm -f drinks_bar atom_supplier molecule_requester
