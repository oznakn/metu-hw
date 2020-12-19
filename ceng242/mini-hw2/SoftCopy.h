/*************************************************************************
 *						DO NOT MODIFY THIS FILE !!!						 *
/************************************************************************/

#ifndef SOFTCOPY_H
#define SOFTCOPY_H

#include "Book.h"

class SoftCopy : public Book {

	private:
		int startPage, endPage;	/* id for the first page and last page, resp.
								   Note that there may not be any page uploaded
								   yet. */

		int numberOfPages;		/* I used this one to hold the book capacity
		 						   It is always equal to endPage-startPage+1.
								   even if there has not been uploaded any page
								   yet. */

		string pageContent;		/* holds the string held by this current softcopy
		                           (not the ones held by its children). */

		SoftCopy* firstHalf;	/* Points to the child softcopy representing its
								   first half. */

		SoftCopy* secondHalf;	/* Points to the child softcopy representing its
								   second half. */

	public:
		/*********************************************************************
		*	Constructor
		*
		* The first @param is ISBN (id number) of the book
		* The second @param is price of the book
		* The third @param is the starting page id
		* The fourth @param is the ending page id
		* Set the "next" variable to NULL.
		*/
		SoftCopy(int, double, int, int);
		/*********************************************************************
		*	Copy Constructor
		*
		* All is deep copy except the pointer variable 'next'
		*/
		SoftCopy(const SoftCopy&);
		/*********************************************************************
		*	Assignment operator
		*
		*  All is deep copy except the pointer variable 'next'
		*/
		SoftCopy& operator=(const SoftCopy&);
		/*********************************************************************
		*	Operator Overload +
		*
		* Adds the pages given in the @param to this SoftCopy object.
		* See the pdf for the example and explanation.
		* You should create a new SoftCopy object in the heap, such that it
		  includes the result of addition. In the final, you should return
		  that newly created heap object.
		  The deletion of the returned heap object will be later done by the
		  admin of the homework.
		* Apply deep copy while adding the pages if you are implementing it via
		  copy operations.
		* Note: Do not touch pointer variable 'next' here.
		*/
		SoftCopy& operator+(const SoftCopy&) const;
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
		* It applies 25% of discount. For example:
		  - If the price is 100 TL, the new price will be 75 TL.
		  - If the price is 60 TL, the new price will be 60 * (75/100) = 45 TL.
		*/
		void Discount();
		/*********************************************************************
		*	UploadPage
		*
		* Constructs the page whose id is given in the second @param, then
		  Uploads the page content given in the first @param.
		* See the pdf for the example and explanation.
		*/
		void UploadPage(string, int);
		/*********************************************************************
		*	Display
		*
		* @return content of the pages in the given interval whose borders are
		  defined by the first and second @param.
		* If there are missing pages (not constructed yet) in the given interval,
		  skip them.
		* Also content of the pages with id 'from' and with id 'to' are added to
		  output (if those pages exist of course).
		* For the output, combine the contents of different pages by pushing
		  a newline character '\n'.
		* See the pdf for the example and explanation.
		*/
		string Display(int from, int to) const;
		/*********************************************************************
		*	Destructor
		*
		* Do NOT delete the pointer variable "next". Instead, set it to NULL.
		* The rest is normal destruction.
		*/
		~SoftCopy();
		/*********************************************************************
		********************* DO NOT TOUCH THE BELOW *************************
		/*********************************************************************
		*
		* Please do NOT do anything about the below methods.
		* Please do NOT touch their declarations.
		* Do NOT implement them. They will implemented by the admin of the HW.
		*/
		friend ostream& operator<<(ostream&, const SoftCopy&);
		friend void AutoPageUploader(string, int, SoftCopy&);
		/*********************************************************************/
};

#endif
