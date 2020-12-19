#ifndef CURVYWALL_H
#define CURVYWALL_H

#include "Wall.h"
#define PI 3.14159265

class CurvyWall : public Wall {
	private:	// Add private members, methods and constructors here as you need


	protected:	// Add protected members, methods and constructors here as you need
		float _radius;
		Particle* _center_particle;

		unsigned char Type() const;
		void ComputeRadius();

	public:		// Do NOT make any modifications below!

		/*********************************************************************
		*									Constructor
		*
		* The first @param is the length of the CurvyWall
		* The second @param is the radius of the CurvyWall
		*/
		CurvyWall(float, float);
		/*********************************************************************
		*									Constructor
		*
		* The first @param is the start point of CurvyWall
		* The second @param is the end point of CurvyWall
		* The third @param is the center point of CurvyWall
		* The CurvyWall object is defined in counterclockwise direction.
		*/
		CurvyWall(const Particle&, const Particle&, const Particle&);
		/*********************************************************************
		*									Copy Constructor
		*
		* Deep copy
		*/
		CurvyWall(const CurvyWall&);
		/*********************************************************************
		*									GetRadius
		*
		* @return the radius of CurvyWall
		*/
		float GetRadius() const;
		/*********************************************************************
		*									GetCenter
		*
		* @return the center point of CurvyWall
		*/
		const Particle& GetCenter() const;
		/*********************************************************************
		*									ComputeLength
		*
		* Computes the length of the CurvyWall object
		* Use PI defined in the macros in case that you need
		*/
		void ComputeLength();
		/*********************************************************************
		*									Clone
		*
		* @return pointer to a copy of this Wall object
		*/
		Wall* Clone() const;
		/*********************************************************************
		*									IsContinuousLinear
		*
		* @return false always
		*/
		bool IsContinuousLinear(const Wall&) const;
		/*********************************************************************
		*									Operator overload
		*
		* Combines this CurvyWall object with the Wall given in the @param
		* @return the combined wall
		*	- The combination can be done only if the wall given in the argument
			is also a CurvyWall such that one wall starts at the point where
			  the other ends and their center is common (coordinates of their
			  center points are the same)
			- On the other hand, the fact that which one of the two walls starts
			  first is not important. Similar to the fact that 3+5 = 5+3.
		* For the cases which do not satisfy above, the method should throw
		  ApplePearException which was defined in Exception.h.
		* The walls to be combined will always be the ones constructed with
		  their start and end points (anomalous examples will not be tested.)
		 */
		const Wall& operator+(const Wall&) const;
		/*********************************************************************
		*									Destructor
		*
		 */
		~CurvyWall();
		/********************************************************************/
};

#endif
