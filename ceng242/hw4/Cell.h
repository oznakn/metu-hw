#ifndef CELL_H
#define CELL_H

#include "Wall.h"
#include "CurvyWall.h"

#include <vector>
class Tissue;	// forward declaration

class Cell {

	private:	// Add private members, methods and constructors here as you need
		int _id;
		vector<Wall*> _walls;
		Tissue* _tissue;

	public: 	// Do NOT make any modifications below!

		/*********************************************************************
		*									Constructor
		*
		* The first @param is the id of the cell
		* The second @param is the cell wall which defines the shape of the cell
		* The third @param is the Tissue that cell belongs to
		*/
		Cell(int, const vector<Wall*>&, Tissue*);
		/*********************************************************************
		*									Copy Constructor
		*
		* Deep copy
		*/
		Cell(const Cell&);
		/*********************************************************************
		*									GetCellID
		*
		* @return the cell id
		*/
		int GetCellID() const;
		/*********************************************************************
		*									GetCellID
		*
		* @return pointer for the tissue that this cell belongs to.
		*/
		Tissue* GetTissue() const;
		/*********************************************************************
		*									GetCellWall
		*
		* @return the cell wall
		*/
		const vector<Wall*>& GetCellWall() const;
		/*********************************************************************
		*									RenewCellWall
		*
		* Replaces the current cell wall with the one given in the argument
		*/
		void RenewCellWall(vector<Wall*>&);
		/*********************************************************************
		*									StrengthenCellWall
		*
		* Creates a new cell wall by connecting the partial walls which can
		  be added with + operator
		* Replaces the old cell wall with the new one
		*/
		void StrengthenCellWall();
		/*********************************************************************
		*									Destructor
		*
		*/
		~Cell();
		/*********************************************************************
		*									Stream Operator
		*
		* Prints cell wall
		* The format is: (p1.x,p1.y) - (p2.x,p2.y) - ... - (pn.x,pn.y)
		  where p1, p2, ..., pn are the the particles on the cell wall.
		* Do NOT add new line character "\n" to the end of the stream.
		*/
		friend ostream& operator<<(ostream&, const Cell&);
		/********************************************************************/
};

#endif
