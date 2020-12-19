#ifndef APPLEPEAR_H
#define APPLEPEAR_H

#include <exception>

using namespace std;

// Do NOT modify this file!

class ApplePearException : public exception {

	/* This exceptin class is used in the summation of Wall and CurvyWall objects
		in case that two non-summable Wall and CurvyWall objects are tried to be
		summed.
	*/

	public:
		virtual const char* what() const throw() {
		return "You can not join different types of walls. In other words, you can not add apple with pear!";
	}
};

class DeadCellException : public exception {

	/* This exceptin class is used while trying to reach a dead cell in some
	   tissue.
	 */

	public:
		virtual const char* what() const throw() {
		return "The cell that your are trying to reach is a dead cell!";
	}
};

class NotBornChildException : public exception {

	/* This exceptin class is used while trying to reach a non-existing child
	   organism of a Squadratic microorganism.
	 */

	public:
		virtual const char* what() const throw() {
		return "The cell that your are trying to reach is not born yet!";
	}
};

#endif
