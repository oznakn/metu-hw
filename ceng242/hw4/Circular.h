#ifndef CIRCULAR_H
#define CIRCULAR_H

#include "MicroOrganism.h"
#include "Tissue.h"

class Circular : public MicroOrganism {

	private:	// Add private members, methods and constructors here as you need
		float _radius;

		unsigned char Type() const;

	public:		// Do NOT make any modifications below!

		/*********************************************************************
		*									Constructor
		*
		* The first @param is the id of the cell
		* The second @param is the radius of the boundary
		*/
		Circular(int, float);
		/*********************************************************************
		*									Destructor
		*
		*/
		~Circular();
		/*********************************************************************
		*									ConnectToCell
		*
		* See the base class explanation.
		*/
		void ConnectToCell(Cell* const);
		/*********************************************************************
		*									DoesFitInto
		*
		* See the base class explanation.
		*/
		bool DoesFitInto(const Cell&) const;
		/*********************************************************************
		*										React
		*
		* This type of microorganism makes the size of the cell get two times
		  larger without changing the center of the cell.
		* As an effect of the enlarging, you should renew the cell walls.
		* The microorganism size does not change.
		*/
		void React();
		/*********************************************************************
		*									DoesContain
		*
		* Special to this type of microorganism
		* Checks whether the given Particle in the @param is inside the
		  this microorganism, or not.
		* @return true if the particle is inside, false otherwise.
		*/
		bool DoesContain(const Particle&) const;
		/*********************************************************************/
};

#endif
