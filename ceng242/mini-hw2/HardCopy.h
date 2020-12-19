/*************************************************************************
 *						DO NOT MODIFY THIS FILE !!!						 *
/************************************************************************/

#ifndef HARDCOPY_H
#define HARDCOPY_H

#include "Book.h"

class HardCopy : public Book {

	private:
		// Nothing special to this class is given.

	public:
		/*********************************************************************
		*	Constructor
		*
		* The first @param is ISBN (id number) of the book
		* The second @param is price of the book
		* Set the "next" variable to NULL.
		*/
		HardCopy(int, double);
		/*********************************************************************
		*	Copy Constructor
		*
		* shallow copy
		*/
		HardCopy(const HardCopy&);
		/*********************************************************************
		*	Clone
		*
		* @return pointer to a copy of this Book object
		*/
		Book* Clone() const;
		/*********************************************************************
		*	Discount
		*
		* Discounts the price of the book.
		* It applies 10% of discount. For example:
		  - If the price is 100 TL, the new price will be 90 TL.
		  - If the price is 60 TL, the new price will 60 * (90/100) = 54 TL.
		*/
		void Discount();
		/*********************************************************************
		*	Destructor
		*
		* Do NOT delete the pointer variable "next". Instead, set it to NULL.
		*/
		~HardCopy();
		/*********************************************************************/
	};

	#endif
