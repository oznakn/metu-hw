#ifndef MAIN_H
#define MAIN_H

#include "Particle.h"
#include "Wall.h"
#include "CurvyWall.h"
#include "Cell.h"
#include "MicroOrganism.h"
#include "Triangular.h"
#include "Circular.h"
#include "Squadratic.h"
#include "Tissue.h"

#include <fstream>
#include <string>

/*********************************************************************
*				DO NOT MODIFY THIS FILE !!!							 *
/********************************************************************/

/*********************************************************************
*				ReadMicroOrganisms
*
* Takes the file name which holds the microorganism data as @param
* Creates the corresponding microorganism objects and holds them in
  vector
* @return the vector
/********************************************************************/
vector<MicroOrganism*> ReadMicroOrganisms(const char* fileName) {

  vector<MicroOrganism*> microOrganisms;
  ifstream moFile(fileName);
	if (moFile.is_open())
  {
	int numberOfMicroOrganisms, id;
	string type;                      // microorganism type
	moFile >> numberOfMicroOrganisms;
	for (int i=0; i<numberOfMicroOrganisms; i++) {
	  moFile >> id;                   // read microorganism id
	  moFile >> type;                 // read microorganism type

	  if (type == "Circular") {       // Circular
		int radius;
		moFile >> radius;
		microOrganisms.push_back(new Circular(id, radius));
	  }
	  else if (type == "Squadratic") { // Squadratic
		float edgeLength;
		moFile >> edgeLength;
		microOrganisms.push_back(new Squadratic(id, edgeLength));
	  }
	  else if (type == "Triangular") { // Triangular
		vector<Wall*> edges;
		float edgeLength[3];
		moFile >> edgeLength[0] >> edgeLength[1] >> edgeLength[2];
		for (int j=0; j<3; j++)
		  edges.push_back(new Wall(edgeLength[j]));
		string RNAsequence;
		moFile >> RNAsequence;
		NucleoBase* RNA = new NucleoBase(RNAsequence, NULL);
		microOrganisms.push_back(new Triangular(id, edges, *RNA));
		for (int j=0; j<3; j++)
		  delete edges[j];
		delete RNA;
	  }
	  else;                           // Unknown type does not exist yet
	}
	moFile.close();
  }
  return microOrganisms;
}

/*********************************************************************
*				ReadCells
*
* Takes the file name which holds the cell data as first @param
* Creates the corresponding cell objects into the Tissue object given
  in the second @param.and holds them in vector
/********************************************************************/
void ReadCells(const char* fileName, Tissue& tissue) {
	ifstream cellsFile(fileName);
	if (cellsFile.is_open())
  {
		int numberOfParticles, numberOfCells, id;
	cellsFile >> numberOfParticles >> numberOfCells;

		// READ PARTICLES
		vector<Particle*> particles;
		for (int i=0; i<numberOfParticles; i++) {
			float x, y;	                              // particle coordinates
			cellsFile >> id >> x >> y;
			particles.push_back(new Particle(x, y));	// it will locate in particles[id]
		}

		// READ CELLS
		for (int i=0; i<numberOfCells; i++) {
				int p1, p2, center, numberOfWalls;      // start point id, end point id, center point id
		cellsFile >> id >> numberOfWalls;       // read the cell id and the number of walls in the cell
				// read walls
				vector<Wall*> cellWall;
				for (int j=0; j<numberOfWalls; j++) {
		  char wallType;
					cellsFile >> wallType;
					if (wallType == 's') {	              // straight wall
						cellsFile >> p1 >> p2;
			Wall* wall = new Wall(*particles[p1], *particles[p2]);
						cellWall.push_back(wall);
					}
					else {                               // curvy wall
			cellsFile >> p1 >> p2 >> center;
			Wall* wall = new CurvyWall(*particles[p1], *particles[p2], *particles[center]);
						cellWall.push_back(wall);
					}
				}
		Cell* cell = new Cell(id, cellWall, &tissue);
				tissue.AddCell(*cell);  // do not delete cell since it is added directly;
		for (int j=0; j<cellWall.size(); j++)
		  delete cellWall[j];
		}

	cellsFile.close();
	for (int i=0; i<numberOfParticles; i++)
	  delete particles[i];
  }
}

#endif
