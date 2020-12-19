#ifndef MICROORGANISM_H
#define MICROORGANISM_H

#include "Wall.h"
#include "CurvyWall.h"
#include "Cell.h"

class MicroOrganism {	// Abstract Shape Class

	protected:	// Add protected members, methods and constructors here as you need
		static const unsigned char TYPE_TRIANGULAR = 1;
		static const unsigned char TYPE_CIRCULAR = 2;
		static const unsigned char TYPE_SQUADRATIC = 3;

		int _id;
		vector<Wall*> _edges;
		Cell* _cell;

		MicroOrganism();
		MicroOrganism(int);

		virtual unsigned char Type() const = 0;

	public:		// Do NOT make any modifications below!

		/*********************************************************************
		*									Constructor
		*
		* The first @param is the microorganism id
		* The second @param is the edges of Wall type
		  The given edges will have been constructed by their lengths and radius
		  (without a definite location)
		*/
		MicroOrganism(int, const vector<Wall*>& edges);
		/*********************************************************************
		*									Stream Operator
		*
		* Prints the location of the microorganism
		* If it is located in a cell, print in the following format:
		  "The microorganism <id1> was successfully placed into the cell <id2>."
		  where id1 is the microorganism id and id2 is the cell id.
		  If the microorganism is not located into any cell, then print:
		  "The microorganism <id> could not be placed into any cell!"
		* Do NOT add new line character "\n" to the end of the stream.
		*/
		friend ostream& operator<<(ostream&, const MicroOrganism&);
		/*********************************************************************
		*									DoesFitInto
		*
		* Checks whether the microorganism fits into the cell given in
		  the @param.
		* @return true if it fits, false otherwise.
		* A microorganism fits into a cell only if their boundary are the same
		  (the same type like triangle, circle or square and the same size).
		* Note that boundary of a cell is always given in counterclockwise, yet
		  edges of a microorganism do not have an order since it is not located
		  until connecting to a compatible cell. Therefore, a triangular
		  microorganism whose edges of length 3, 5, 4 units respectively may
		  connect to a triangle cell whose cell wall is defined with the walls
		  of length 3, 4, 5. You can consider it as turning the other face of
		  the microorganism to the front.
		* Also, do not forget to check the angels between the edges of a cell
		  while trying to place Squadratics. Checking only the edge lengths
		  is not sufficient since edges of equilateral quadrangles are also
		  equal, yet the inner angles are not 90 degrees.
		* Hint: Strengthen cell walls before checking its edges and angles.
		*/
		virtual bool DoesFitInto(const Cell&) const = 0;
		/*********************************************************************
		*									ConnectToCell
		*
		* Connects the microorganism to the cell given in the @param
		* It is assummed that cell-microorganism comptability was checked
		  before calling this method, and the cell is compatible with
		  the microorganism.
		* Saves the cell pointer.
		* After connection, microorganism has a definite position which is
		  the same location the cell is on. It is your call to save other
		  connection effects like updating the walls/edges of the microorganism
		  with the definitely-located walls or defining vertices of it.
		*/
		virtual void ConnectToCell(Cell* const cell) = 0;
		/*********************************************************************
		*									React
		*
		* Functions depending on the microorganism type.
		* Results affect the microorganism and the cell that it connects to.
		* React method is called only when the microorganism is inside a cell.
		*/
		virtual void React() = 0;
		/*********************************************************************
		*									Destructor
		*
		*/
		virtual ~MicroOrganism() = 0;
		/*********************************************************************/
};

#endif
