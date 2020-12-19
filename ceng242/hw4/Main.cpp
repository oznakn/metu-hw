#include "Main.h"

// How to compile:
// g++ -ansi -pedantic Particle.cpp Wall.cpp CurvyWall.cpp Cell.cpp MicroOrganism.cpp NucleoBase.cpp Triangular.cpp Circular.cpp Squadratic.cpp Tissue.cpp Main.cpp -o hw4
// or by Makefile

/******************************************************************************/
/*                               WALL EXPERIMENTS                             */
/******************************************************************************/
void Wall_Experiments() {
	// Particle Experiments
	Particle* p0 = new Particle(0,0);
	Particle* p1 = new Particle(0,3);
	Particle* q0 = new Particle(0,3);
	Particle* q1 = new Particle(0,5);

	cout <<	"TEST NO 1 - Particle:: FindDistance()" << endl;
	cout << p0->FindDistance(*p1) << endl;
	cout << q1->FindDistance(*q0) << endl;
	cout << "The answer key: " << 3 << " | " << 2 << endl << endl;

	cout << "TEST NO 2 - Particle:: operator==" << endl;
	cout << (*p1 == *q0) << endl;
	Particle* px = new Particle(2,0);
	cout << (*px == *q0) << endl;
	cout << (*q0 == *px) << endl;
	cout << "The answer key: " << "1| 0 | 0"<< endl << endl;

	// Wall Experiments
	Wall* wallp = new Wall(*p0, *p1);
	Wall* wallq = new Wall(*q0, *q1);
	cout << "TEST NO 3 - Wall:: IsContinuousLinear & operator+" << endl;
	cout << (wallp->GetFinalParticle() == wallq->GetInitialParticle()) << endl;
	cout << wallp->IsContinuousLinear(*wallq) << endl;
	const Wall* wall = &(*wallp + *wallq);
	cout << wall->GetLength() << endl;
	cout << "The answer key: " << "1 | 1 | 5"<< endl << endl;

	cout << "TEST NO 4 - Wall:: ApplePearException" << endl;
	Particle* qx = new Particle(1,4);
	Wall* curvyWall = new CurvyWall(*q0, *q1, *qx);
	try {
		const Wall* apple_pear = &(*curvyWall + *wallp);
		delete apple_pear;
	} catch (ApplePearException e) {
		cout << e.what() << endl << endl;
	}
	cout << "The answer key: Should print ApplePearException message." << endl << endl;

	delete p0;
	delete p1;
	delete q0;
	delete q1;
	delete px;
	delete qx;

	delete wallp;
	delete wallq;
	delete wall;
	delete curvyWall;
}

/******************************************************************************/
/*                               CELL EXPERIMENTS                             */
/******************************************************************************/
void Cell_Experiments() {

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

}
/******************************************************************************/
/*                          MICROORGANISM EXPERIMENTS                         */
/******************************************************************************/
void MicroOrganism_Experiments(const char* cellFile, const char* MicroOrganismFile) {
	// Read MicroOrganisms
	vector<MicroOrganism*> microOrganisms = ReadMicroOrganisms(MicroOrganismFile);

	// Read Cells
	Tissue* tissue = new Tissue();
	ReadCells(cellFile, *tissue);

	// Interact microorganisms with cells
	cout << "TEST NO 7 - Tissue:: Interact() which includes MicroOrganism:: DoesFitInto() and ConnectToCell()" << endl;
	tissue->Interact(microOrganisms);
	for (int i=0; i < microOrganisms.size(); i++)
		cout << *microOrganisms[i] << endl;
	cout << "The answer key: " << endl;
	cout << "\t Microorganism 0 into Cell 6" << endl;
	cout << "\t Microorganism 1 into NONE  " << endl;
	cout << "\t Microorganism 2 into Cell 4" << endl;
	cout << "\t Microorganism 3 into Cell 2" << endl;
	cout << "\t Microorganism 4 into Cell 1" << endl;
	cout << "\t Microorganism 5 into Cell 0" << endl << endl;

	// Make microorganisms reacted
	cout << "TEST NO 8 - Microorganism:: React() " << endl;

	cout << "\t Microorganism 0 reacts:" << endl;
	microOrganisms[0]->React();																																				// A circular microorganism reaction
	cout << "Radius is: " << ((CurvyWall*)tissue->GetCell(6).GetCellWall()[0])->GetRadius() << endl;	// Did cell walls get larger?
	cout << "Center is: " << ((CurvyWall*)tissue->GetCell(6).GetCellWall()[0])->GetCenter() << endl;	// Center does not change
	cout << "Cell wall is: " << tissue->GetCell(6) << endl;																						// Any point on the cell (Assuming it is strengthened, not necessarily)
	cout << "The answer key: Radius is 10 and Center is (8,0). The rest should be fitting cell wall(s)." << endl << endl;

	cout << "\t Microorganism 2 reacts:" << endl;
	microOrganisms[2]->React();																																				// A circular microorganism reaction
	cout << "Radius is: " << ((CurvyWall*)tissue->GetCell(4).GetCellWall()[0])->GetRadius() << endl;	// Did cell walls get larger?
	cout << "Center is: " << ((CurvyWall*)tissue->GetCell(4).GetCellWall()[0])->GetCenter() << endl;	// Center does not change
	cout << "Cell wall is: " << tissue->GetCell(4) << endl;																						// Any point on the cell (Assuming it is strengthened, not necessarily)
	cout << "The answer key: Radius is 4 and Center is (1,-5). The rest should be fitting cell wall(s)." << endl << endl;

	cout << "\t Microorganism 3 reacts:" << endl;
	microOrganisms[3]->React();																																				// A triangular microorganism reaction
	try {
		const Cell cell =  tissue->GetCell(2);
	} catch (DeadCellException e) {
		cout << e.what() << endl << endl;
	}
	cout << "The answer key: Should print DeadCellException message." << endl << endl;

	cout << "\t Microorganism 4 reacts:" << endl;
	microOrganisms[4]->React();																																				// A triangular microorganism reaction
	try {
		const Cell cell =  tissue->GetCell(1);
	} catch (DeadCellException e) {
		cout << e.what() << endl << endl;
	}
	cout << "The answer key: Should print DeadCellException message." << endl << endl;

	cout << "\t Microorganism 5 reacts:" << endl;
	microOrganisms[5]->React();																																				// A squadratic microorganism reaction
	try {		// Child request-1
		Squadratic& child = ((Squadratic*)microOrganisms[5])->GetChild(1.5, 3, 0, 1.5);
		child.React();

		try {	// Child request-2
			Squadratic& grandchild = ((Squadratic*)microOrganisms[5])->GetChild(0.75, 1.5, 1.5, 2.25);
		}
		catch (NotBornChildException e) {
			cout << "First child request is ok, yet for the second child request you got:" << endl;
			cout << e.what() << endl << endl;
		}

		try {	// Child request-3
			Squadratic& grandchild = ((Squadratic*)microOrganisms[5])->GetChild(1.5, 2.25, 0.75, 1.5);
		} catch (NotBornChildException e) {
			cout << "First child request is ok, yet for the third child request you got:" << endl;
			cout << e.what() << endl << endl;
		}
	} catch (NotBornChildException e) {
		cout << "You failed in requesting the first child:" << endl;
		cout << e.what() << endl << endl;
	}
	cout << "The answer key: Should print NotBornChildException message only for the second child request." << endl << endl;

	// Test the methods which are specialized to microorganism types
	cout << "TEST NO 9 - Circular:: DoesContain() " << endl;
	Particle p(10,1);
	Particle q(-2,-5);
	cout << ((Circular*)microOrganisms[0])->DoesContain(p) << endl;
	cout << ((Circular*)microOrganisms[2])->DoesContain(p) << endl;
	cout << "The answer key: 1 | 0 " << endl << endl;


	cout << "TEST NO 10 - Triangular:: Mutate() " << endl;
	cout << "RNA before the mutation: " << ((Triangular*)microOrganisms[4])->GetRNA() << endl;
	((Triangular*)microOrganisms[4])->Mutate((Triangular&)*microOrganisms[3]);
	cout << "RNA after the mutation: " << ((Triangular*)microOrganisms[4])->GetRNA() << endl;
	cout << "The answer key: AGSGGAUUUASGAGSAAUAG should change to AGSGGAUUASGUUAG." << endl << endl;

	// Deletes
	delete tissue;
	for (int i=0; i < microOrganisms.size(); i++)
		delete microOrganisms[i];

}

/******************************************************************************/
/*                                    MAIN()                                  */
/******************************************************************************/
int main(int argc, char* argv[]) {

	Wall_Experiments();
	Cell_Experiments();
	MicroOrganism_Experiments("/home/oznakn/Documents/METU/20192/242/hw4/input/cells.txt", "/home/oznakn/Documents/METU/20192/242/hw4/input/microorganisms.txt");	// argv[1] = "input/cells.txt",	argv[2] = "input/microorganisms.txt"

	return 0;

}
