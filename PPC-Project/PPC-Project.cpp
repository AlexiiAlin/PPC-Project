// PPC-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Test git

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "mpi.h"

#pragma comment (lib, "msmpi.lib")
using namespace std;

class Street {
public:
	Street(Street *other) {
		if (this != other) { // self-assignment check expected
			this->nr = other->nr;
			this->beginStreet = other->beginStreet;
			this->endStreet = other->endStreet;
			this->way = other->way;
		}
	}

	int nr, beginStreet, endStreet, way;
	Street(int nr, int beginStreet, int endStreet, int way) : nr(nr), beginStreet(beginStreet), endStreet(endStreet),
		way(way) {}
	Street() {}

	Street& operator=(const Street& other) // copy assignment
	{
		if (this != &other) { // self-assignment check expected
			this->nr = other.nr;
			this->beginStreet = other.beginStreet;
			this->endStreet = other.endStreet;
			this->way = other.way;
		}
		return *this;
	}

	void show() {
		cout << this->nr << " " << this->beginStreet << " " << this->endStreet << " " << ((this->way == 1) ? "One way" : "Two way") << endl;
	}
};

int createNumber(int nr, int street) {
	int oddNumber, evenNumber;
	if (nr % 2 == 0) {
		oddNumber = street;
		evenNumber = nr;
	}
	else {
		oddNumber = nr;
		evenNumber = street;
	}

	return oddNumber * 10 + evenNumber;
}

int main() {
	ifstream f;
	f.open("ppc.in");
	int nStreets;
	f >> nStreets;

	Street* streets = (Street *)malloc(nStreets * sizeof(Street));

	for (int i = 0; i < nStreets; i++) {
		int nr, beginStreet, endStreet, way;
		f >> nr >> beginStreet >> endStreet >> way;
		streets[i] = new Street(nr, beginStreet, endStreet, way);
	}

	// Calculate number of intersections ( number of nodes in the graph )
	// We assume the array is sorted (for now)

	int nrNodes = 0;
	set<int> visitedStreets;
	int streetNr = streets[0].nr;
	set<int> intersections;

	for (int i = 0; i < nStreets; i++) {
		if (streetNr != streets[i].nr) {
			visitedStreets.insert(streetNr);
			nrNodes = nrNodes + intersections.size();
			intersections.clear();
			streetNr = streets[i].nr;
			i--;
		}
		else {
			const bool beginExists = visitedStreets.find(streets[i].beginStreet) != visitedStreets.end();
			const bool endExists = visitedStreets.find(streets[i].endStreet) != visitedStreets.end();
			if (!beginExists) {
				intersections.insert(streets[i].beginStreet);
			}
			if (!endExists) {
				intersections.insert(streets[i].endStreet);
			}
		}
	}

	// Construct the mappingIntersections

	map<int, int> mappingIntersections;
	int mappingNode = 0;
	for (int i = 0; i < nStreets; i++) {
		int firstNr = createNumber(streets[i].nr, streets[i].beginStreet);
		int secondNr = createNumber(streets[i].nr, streets[i].endStreet);

		map<int, int>::iterator it;
		bool firstNrExists = false, secondNrExists = false;
		for (it = mappingIntersections.begin(); it != mappingIntersections.end(); it++)
		{
			if (it->first == firstNr) {
				firstNrExists = true;
			}
			if (it->first == secondNr) {
				secondNrExists = true;
			}
		}

		if (!firstNrExists) {
			mappingIntersections.insert(pair<int, int>(firstNr, mappingNode));
			mappingNode++;
		}
		if (!secondNrExists) {
			mappingIntersections.insert(pair<int, int>(secondNr, mappingNode));
			mappingNode++;
		}
	}

	cout << "Mapping intersections:\n\n";
	cout << "==============\n";

	map<int, int>::iterator it;
	for (it = mappingIntersections.begin(); it != mappingIntersections.end(); it++)
	{
		cout << "key: " << it->first << " value: " << it->second << endl;
	}

	cout << "==============\n\n\n";


	// Construct the adjacency matrix

	int* M = (int *)malloc(nrNodes*nrNodes*sizeof(int));

	for (int i = 0; i < nrNodes; i++) {
		for (int j = 0; j < nrNodes; j++) {
			M[i*nrNodes + j] = 0;
		}
	}

	for (int i = 0; i < nStreets; i++) {
		int firstNr = createNumber(streets[i].nr, streets[i].beginStreet);
		int secondNr = createNumber(streets[i].nr, streets[i].endStreet);

		int firstMappedNr = mappingIntersections.at(firstNr);
		int secondMappedNr = mappingIntersections.at(secondNr);

		// firstNr -> secondNr (depends on way -> / <-> )
		M[firstMappedNr*nrNodes + secondMappedNr] = 1;
		if (streets[i].way == 2) {
			M[secondMappedNr*nrNodes + firstMappedNr] = 1;
		}
	}


	// Display adjacency matrix

	cout << "Adjacency matrix:\n\n";

	cout << "   ";
	for (int i = 0; i < nrNodes; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (int i = 0; i <= nrNodes; i++) {
		cout << "--";
	}
	cout << endl;
	for (int i = 0; i < nrNodes; i++) {
		cout << i << "| ";
		for (int j = 0; j < nrNodes; j++) {
			cout << M[i*nrNodes + j] << " ";
		}
		cout << endl;
	}


	// Test the MPI Environment:
	cout << endl << endl;
	int rank, size;
	MPI_Init(NULL, NULL);	/* starts MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	/* get current process id */
	MPI_Comm_size(MPI_COMM_WORLD, &size);	/* get number of processes */
	printf("Hello world from process %d of %d\n", rank, size);
	MPI_Finalize();
	return 0;
}
