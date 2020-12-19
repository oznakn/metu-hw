/******************************************************************************/
/*                               WALL EXPERIMENTS                             */
/******************************************************************************/

#include "Particle.h"
#include "Wall.h"
#include "CurvyWall.h"

int main(int argc, char* argv[]) {

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

	return 0;

}
