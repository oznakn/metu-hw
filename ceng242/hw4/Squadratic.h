#ifndef SQUADRATIC_H
#define SQUADRATIC_H

#include "MicroOrganism.h"
#include "Tissue.h"

class Squadratic : public MicroOrganism {

	private:	// Add private members, methods and constructors here as you need
		float _edge_length;
		vector<Squadratic*> _childs;

		unsigned char Type() const;

	public:		// Do NOT make any modifications below!

		/*********************************************************************
		*									Constructor
		*
		* The first @param is the id of the cell
		* The second @param is the edge length in the boundary
		*/
		Squadratic(int, float);
		/*********************************************************************
		*									Copy Constructor
		*
		* Deep copy
		*/
		Squadratic(const Squadratic&);
		/*********************************************************************
		*									Destructor
		*
		*/
		~Squadratic();
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
		*									React
		*
		* This type of microorganism proceeds into meiosis division as follows:
		  The current squadratic microorganism becomes a parent of 4 children
		  such that each child is a new squadratic microorganism whose edge
		  length is half of the parent organism's edge length.
		* Id of the child microorganisms does not matter. Give anything you
		  want different than the previous microorganism ids.
		* Each child squadratic positions into a different corner of the cell
		  such that one of their top left, bottom left, top right and bottom
		  right corners match with one of the corresponding corner of the cell.
		* React() method can also be called for child microorganisms.
		*/
		void React();
		/*********************************************************************
		*									GetChild
		*
		* Special to this type of microorganism
		* @return the child microorganism which is located between the minimum
		  and maximum x and y coordinates given in the @param.
		* The first @param is the minimum x border.
		* The second @param is the maximum x border.
		* The third @param is the minimum y border.
		* The fourth @param is the maximum y border.
		* This method either directly returns the child whose border coordinates
		  exactly equal to the parameters, or throws NotBornChildException in
			case that the asked child was created yet (the microorganism may
		  have not been divided upto that level.)
		*/
		Squadratic& GetChild(float, float, float, float) const;
		/*********************************************************************/
};

#endif
