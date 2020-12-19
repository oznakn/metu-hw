/********************************************************************************/
/*                               CELL EXPERIMENTS                               */
/*																																							*/
/*	Note: Includes a little amount of MicroOrganism tests, DoesFitInto() method */
/********************************************************************************/
#include "Particle.h"
#include "Wall.h"
#include "CurvyWall.h"
#include "Cell.h"
#include "MicroOrganism.h"
#include "Triangular.h"
#include "Circular.h"
#include "Squadratic.h"
#include "Tissue.h"
#include <string>

int main() {

	int i;
	Tissue* tissue = new Tissue();

	// Square between (0,0) and (3,3)
	vector<Particle*> p;
	p.push_back(new Particle(0,3));
	p.push_back(new Particle(1,3));
	p.push_back(new Particle(3,3));
	p.push_back(new Particle(3,0));
	p.push_back(new Particle(0,0));
	p.push_back(new Particle(0,1));
	p.push_back(new Particle(0,2));

	vector<Wall*> walls;
	walls.push_back(new Wall(*p[6], *p[5]));
	walls.push_back(new Wall(*p[5], *p[4]));
	walls.push_back(new Wall(*p[4], *p[3]));
	walls.push_back(new Wall(*p[3], *p[2]));
	walls.push_back(new Wall(*p[2], *p[1]));
	walls.push_back(new Wall(*p[1], *p[0]));
	walls.push_back(new Wall(*p[0], *p[6]));

	cout << "TEST NO 5 - Cell:: StrengthenCellWall()" << endl;
	Cell* cell = new Cell(0, walls, tissue);
	cout << "Cell wall coordinates of Square before Strengthening:" << endl;
	cout << *cell << endl;
	cout << "The answer key: " << "(0,2)-(0,1)-(0,0)-(3,0)-(3,3)-(1,3)-(0,3)"<< endl;

	cout << "Cell wall coordinates of Square after Strengthening:" << endl;
	cell->StrengthenCellWall();
	cout << *cell << endl;
	cout << "The answer key: " << "(0,3)-(0,0)-(3,0)-(3,3)"<< endl << endl;

	// Circle with center (1.5,1.5) and radius 1.5
	vector<Particle*> q;
	q.push_back(new Particle(1.5, 1.5));
	q.push_back(new Particle(1.5, 0));
	q.push_back(new Particle(1.5, 3));

	vector<Wall*> circleWalls;
	circleWalls.push_back(new CurvyWall(*q[1], *q[2], *q[0]));
	circleWalls.push_back(new CurvyWall(*q[2], *q[1], *q[0]));

	Cell* cellCircle = new Cell(1, circleWalls, tissue);
	cout << "Cell wall coordinates of Circle after Strengthening:" << endl;
	cellCircle->StrengthenCellWall();
	cout << *cellCircle << endl;
	cout << "The answer key: " << "(1.5,3)"<< endl << endl;

	// Triangle1
	vector<Particle*> s;
	s.push_back(new Particle(1, 3));
	s.push_back(new Particle(5, 0));
	s.push_back(new Particle(1, 0));

	vector<Wall*> t1Walls;
	t1Walls.push_back(new Wall(*s[2], *s[1]));
	t1Walls.push_back(new Wall(*s[1], *s[0]));
	t1Walls.push_back(new Wall(*s[0], *s[2]));

	Cell* cellT1 = new Cell(3, t1Walls, tissue);

	// Triangle2
	vector<Particle*> t;
	t.push_back(new Particle(2,0));
	t.push_back(new Particle(-1, 4));
	t.push_back(new Particle(5, 4));

	vector<Wall*> t2Walls;
	t2Walls.push_back(new Wall(*t[2], *t[1]));
	t2Walls.push_back(new Wall(*t[1], *t[0]));
	t2Walls.push_back(new Wall(*t[0], *t[2]));

	Cell* cellT2 = new Cell(4, t2Walls, tissue);

	// Unknown
	vector<Particle*> r;
	r.push_back(new Particle(-2,5));
	r.push_back(new Particle(-2,-5));
	r.push_back(new Particle(-5,0));

	vector<Wall*> unknownWalls;
	unknownWalls.push_back(new Wall(*r[0], *r[1]));
	unknownWalls.push_back(new CurvyWall(*r[1], *r[0], *r[2]));

	Cell* unknownCell = new Cell(5, unknownWalls, tissue);

	// Triangular MicroOrganism
	vector<Wall*> tomWalls;
	tomWalls.push_back(new Wall(4));
	tomWalls.push_back(new Wall(5));
	tomWalls.push_back(new Wall(3));

	string stringChain = "SGAAAGSUSUGAUAGGUASSGUSAA";
	NucleoBase* RNA = new NucleoBase(stringChain, NULL);
	Triangular* tom = new Triangular(0, tomWalls, *RNA);

	cout << "TEST NO 6 - MicroOrganism:: DoesFitInto()" << endl;
	cout << tom->DoesFitInto(*cell) << endl;
	cout << tom->DoesFitInto(*cellCircle) << endl;
	cout << tom->DoesFitInto(*cellT1) << endl;
	cout << tom->DoesFitInto(*cellT2) << endl;
	cout << tom->DoesFitInto(*unknownCell) << endl;
	cout << "The answer key: " << "0 | 0 | 1 | 0 | 0" << endl << endl;

	// DELETE
	delete tom;
	delete cell;
	delete cellCircle;
	delete cellT1;
	delete cellT2;
	delete unknownCell;
	delete RNA;
	delete tissue;

	for (i=0; i < walls.size(); i++)
		delete walls[i];
	for (i=0; i < circleWalls.size(); i++)
		delete circleWalls[i];
	for (i=0; i < t1Walls.size(); i++)
		delete t1Walls[i];
	for (i=0; i < t2Walls.size(); i++)
		delete t2Walls[i];
	for (i=0; i < unknownWalls.size(); i++)
		delete unknownWalls[i];
	for (i=0; i < tomWalls.size(); i++)
		delete tomWalls[i];

	for (i=0; i < p.size(); i++)
		delete p[i];
	for (i=0; i < q.size(); i++)
		delete q[i];
	for (i=0; i < s.size(); i++)
		delete s[i];
	for (i=0; i < t.size(); i++)
		delete t[i];
		for (i=0; i < r.size(); i++)
			delete r[i];

	return 0;
}
