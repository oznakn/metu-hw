#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <cmath>

using namespace std;

#define EPSILON 0.01

class Particle {

	private:	// Add private members, methods and constructors here as you need
		float _x;
		float _y;

	public:		// Do NOT make any modifications below!

		/*********************************************************************
		*	Constructor
		*
		* The first @param is the x-coordinate of the point
		* The second @param is the y-coordinate of the point
		*/
		Particle(float, float);
		/*********************************************************************
		*	Copy constructor
		*
		*/
		Particle(const Particle&);
		/*********************************************************************
		*	X()
		*
		* @return x-coordinate of the point
		*/
		float X() const;
		/*********************************************************************
		*	Y()
		*
		* @return y-coordinate of the point
		*/
		float Y() const;
		/*********************************************************************
		*	FindDistance
		*
		* Computes the Eucledean distance of this 2D Particle object to another
		  particle given in the @param
		* @return the distance
		*/
		float FindDistance(const Particle&) const;
		/*********************************************************************
		*	Operator Overload
		*
		* Checks whether the x and y coordinates of this 2D Particle object is
		  equal to those of the particle given in the @param
		* @return true if they are equal, @return false if not
		* Use EPSILON given in the macros to compare the float values.
		*/
		bool operator== (const Particle&) const;
		/*********************************************************************
		*									Stream Operator
		* Prints particle coordinates
		* The format is: (p.x,p.y) where p is the particle in the @param
		* Do NOT add new line character "\n" to the end of the stream.
		*/
		friend ostream& operator<<(ostream&, const Particle&);
		/********************************************************************/
};

#endif
