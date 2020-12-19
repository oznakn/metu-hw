/*************************************************************************
 *						DO NOT MODIFY THIS FILE !!!						 *
/************************************************************************/

#ifndef BOOKSTORE_H
#define BOOKSTORE_H

#include "SoftCopy.h"
#include "HardCopy.h"
#include <vector>

class BookStore {

	private:
		string name;		// name of the bookstore
		int size;			// number of books in a bookstore
		Book* initialBook;	// points to initial book in the bookstore

		/* Assuming all the books are related to each other with a singly
		linked list and ordered according to increasing order of their ISBNs.
		*/

	public:
		/*********************************************************************
		*	Constructor
		*
		* The first @param is the name of the BookStore object.
		* Equalize the initialBook to NULL.
		*/
		BookStore(string);
		/*********************************************************************
		*	Copy Constructor
		*
		* Deep copy
		*/
		BookStore(const BookStore&);
		/*********************************************************************
		*	AddNewBook
		*
		* Adds the book given in the @param to the right position of the linked
		  list holding the books in this BookStore object.
		*/
		void AddNewBook(const Book&);
		/*********************************************************************
		*	GetBookList
		*
		* @return the head object of the linked list consisting of books whose
		  ISBNs are in the interval defined by the first and the second @param.
		*/
		Book& GetBookList(int, int);
		/*********************************************************************
		*	Destructor
		*
		*/
		~BookStore();
		/*********************************************************************/

};

#endif
