/*************************************************************************
 *						DO NOT MODIFY THIS FILE !!!						 *
/************************************************************************/

#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Book {

	protected:
		int ISBN;				// holds ISBN
		double price;			// holds price
		Book* next;				// points to the next book whose ISBN is
		Book();					/* Default constructor: only sets the
								   pointer variable 'next' to NULL */

	public:

		/*********************************************************************
		*	Constructor
		*
		* The first @param is ISBN (id number) of the book
		* The second @param is price of the book
		* Set the "next" variable to NULL.
		*/
		Book(int, double);
		/*********************************************************************
		*									GetISBN
		*
		* @return the ISBN.
		*/
		int GetISBN() const;
		/*********************************************************************
		*									GetPrice
		*
		* @return the price.
		*/
		double GetPrice() const;
		/*********************************************************************
		*									GetNext
		*
		* @return the pointer variable 'next'.
		*/
		Book* GetNext() const;
		/*********************************************************************
		*									SetNext
		*
		* Sets the pointer variable "next" to a pointer which points to a book
		  object whose value is the same with the one given in the @param.
		*/
		void SetNext(const Book&);
		/*********************************************************************
		*									SetNext Overload
		*
		* Sets the pointer variable "next" to NULL.
		*/
		void SetNext();
		/*********************************************************************
		*									Operator Overload: <
		*
		* @return true if ISBN of this book is lower than ISBN of the book
		  given in the @param, returns false otherwise.
		*/
		bool operator<(const Book&) const;
		/*********************************************************************
		*									Operator Overload: <
		*
		* @return true if ISBN of this book is lower than the @param, returns
		  false otherwise.
		*/
		bool operator<(int ISBN) const;
		/*********************************************************************
		*									Operator Overload: >
		*
		* @return true if ISBN of this book is higher than the @param, returns
		  false otherwise.
		*/
		bool operator>(int ISBN) const;
		/*********************************************************************
		*									Clone
		*
		* @return pointer to a copy of this Book object
		*/
		virtual Book* Clone() const = 0;
		/*********************************************************************
		*									Discount
		*
		* Discounts the prices of the books.
		* For HardCopies, it applies 10% of discount. For example:
		  - If the price is 100 TL, the new price will be 90 TL.
		  - If the price is 60 TL, the new price will 60 * (90/100) = 54 TL.
		* For SoftCopies, it applies 25% of discount. For example:
		  - If the price is 100 TL, the new price will be 75 TL.
		  - If the price is 60 TL, the new price will be 60 * (75/100) = 45 TL.
		* Note that this method returns nothing. However, its effect can be
		  understood by the ShowContent() method.
		*/
		virtual void Discount() = 0;
		/*********************************************************************
		*									Destructor
		*
		* Do NOT delete the pointer variable "next". Instead, set it to NULL.
		*/
		virtual ~Book() = 0;
		/*********************************************************************/
};

#endif
