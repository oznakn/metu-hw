#ifndef TRIANGULAR_H
#define TRIANGULAR_H

#include "MicroOrganism.h"
#include "NucleoBase.h"
#include "Tissue.h"

class Triangular : public MicroOrganism {

	private:	// Add private members, methods and constructors here as you need
		NucleoBase* _nucleo_base;

		unsigned char Type() const;

	public:		// Do NOT make any modifications below!

		/*********************************************************************
		*									Constructor
		*
		* The first @param is the id of the cell
		* The second @param is the vector of edges
		* The third parameter is a NucleoBase object that defining its RNA.
		*/
		Triangular(int, const vector<Wall*>&, const NucleoBase&);
		/*********************************************************************
		*									Destructor
		*
		*/
		~Triangular();
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
		* This type of microorganism kills the cell that it connects to.
		* Note that while you are deleting the cell, you should also make the
		* Tissue noticed that the corresponding cell was killed.
		*/
		void React();
		/*********************************************************************
		*									GetRNA
		*
		* Special to this type of microorganism
		* @return the start node of the NucleoBase sequence which holds the RNA
		  of the microorganism
		*/
		const NucleoBase& GetRNA() const;
		/*********************************************************************
		*									Mutate
		*
		* Special to this type of microorganism
		* Mutate the RNA of this Triangular microorganism by crossing over with
		  RNA of the Triangular microorganism given in the @param.
		* Crossing over is done as follows:
		  Both RNA is compared in both ways: from the beginning to end and from
		  end to beginning. When they start to differ, the corresponding G-S
		  and A-U nucleobases in the differing part of both RNA are interchanged.
		  For example:
		  - Let RNA-1 be ASGAGGAUUGSUAGSSSAAG
		    (make readable: ASGAG GAUUGS UAGSSSAAG)
		  - Let RNA-2 be ASGAGSSGAUUUAGSSSAAG
		    (make readable: ASGAG SSGAUU UAGSSSAAG)
		  Since ASGAG part at the beginning and UAGSSSAAG part at the end are
		  the same in both RNA, those parts are held. For the parts remaining
		  in the middle, the corresponding G-S nucleobases and the corresponding
		  A-U nucleobases are interchanged. Other type of correspondences are
			removed from the chain (Deleted). At the end:
		  - RNA-1 becomes ASGAGSAUAGSSSAAG
		    (make readable: ASGAG SA UAGSSSAAG)
		  - RNA-2 becomes ASGAGGUUAGSSSAAG
		    (make readable: ASGAG GU UAGSSSAAG)
		* You can assume that at least 1 character from the beginning and the end
			will be the same for both RNA sequence (Just for you not to struggle
			with the so detailed implementation). Also, you can assume that at least
			1 character of RNAs will differ.
		*/
		void Mutate(Triangular&);
		/*********************************************************************/
};

#endif
