#ifndef NUCLEOBASE_H
#define NUCLEOBASE_H

#include <iostream>
class Triangular;
using namespace std;

class NucleoBase {
	/* This class represents a linked list to express RNA and will
		be used only by Triangular microorganisms */
	friend class Triangular;

	private:	// Add private members, methods and constructors here as you need
		char _value;
		NucleoBase* _prev;
		NucleoBase* _next;

		NucleoBase(const NucleoBase&, NucleoBase*);

	public:		// Do NOT make any modifications below!

		/*********************************************************************
		*									Constructor
		*
		* The first @param is the whole RNA sequence
		* The second parameter is a NucleoBase pointer addressing the previous
		  NucleoBase object. For the initial nucleobase this is NULL.
		*/
		NucleoBase(string, NucleoBase*);
		/*********************************************************************
		*								Copy Constructor
		*
		* Deep copy
		*/
		NucleoBase(const NucleoBase&);
		/*********************************************************************
		*									Destructor
		*
		*/
		~NucleoBase();
		/*********************************************************************
		*									GetLast
		*
		* @return pointer to the last element of NucleoBase chain.
		*/
		NucleoBase* GetLast();
		/*********************************************************************
		*									Stream Operator
		* Prints the data in NucleoBase chain.
		* Prints the char data of each NucleoBase object starting from the
		  given NucleoBase in the @param upto the last element of the chain.
		* Do NOT add new line character "\n" to the end of the stream.
		*/
		friend ostream& operator<<(ostream&, const NucleoBase&);
		/********************************************************************/
};

#endif
