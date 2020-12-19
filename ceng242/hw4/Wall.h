#ifndef WALL_H
#define WALL_H

#include "Particle.h"
#include "Exception.h"

class CurvyWall;

class Wall {
	friend class CurvyWall;

	protected:	// Add protected members, methods and constructors here as you need
		const static unsigned char TYPE_STRAIGHT = 0;
		const static unsigned char TYPE_CURVY   = 1;

		float _length;
		Particle* _start_particle;
		Particle* _end_particle;

		virtual unsigned char Type() const;
		float Slope() const;

		Wall();

	public:		// Do NOT make any modifications below!

		/*********************************************************************
		*									Constructor
		*
		* @param is the length of the Wall
		*/
		Wall(float);
		/*********************************************************************
		*									Constructor
		*
		* The first @param is the start point of Wall
		* The second @param is the end point of Wall
		* The Wall object is defined in counterclockwise direction.
		*/
		Wall(const Particle&, const Particle&);
		/*********************************************************************
		*									Copy constructor
		*
		* Deep copy
		*/
		Wall(const Wall&);
		/*********************************************************************
		*									GetInitialParticle
		*
		* @return the start point of Wall
		* This method will be called only for the walls construted with their start
		  and end points
		*/
		const Particle& GetInitialParticle() const;
		/*********************************************************************
		*									GetFinalParticle
		*
		* @return the end point of Wall
		* This method will be called only for the walls construted with their start
		  and end points
		*/
		const Particle& GetFinalParticle() const;
		/*********************************************************************
		*									FindAngleBetween
		*
		* @return the angle between this Wall object and the one given in the @param
		*/
		float FindAngleBetween(const Wall&) const;
		/*********************************************************************
		*									GetLength
		*
		* @return the length of Wall
		*/
		float GetLength() const;
		/*********************************************************************
		*									ComputeLength
		*
		* Computes the length of the Wall object
		*/
		virtual void ComputeLength();
		/*********************************************************************
		*									Clone
		*
		* @return pointer to a copy of this Wall object
		*/
		virtual Wall* Clone() const;
		/*********************************************************************
		*									IsContinuousLinear
		*
		* Checks whether the end points of this Wall object and the end points
		  of the Wall given in the @param are linear and the walls are continuous
		* @return true if they are linear and continuous, false otherwise
		* The method returns always false in case that the wall given in the
		  argument is a curvy wall
		* Two walls are linear only if they are straight walls and their slopes
		  are equal (Be cautious while calculating slope of the vertical lines!)
		* Two walls are continuous only if one starts at the point where the
		  other ends (the fact that which one of the two walls starts first is
		  not important.)
		* The walls to be combined will always be the ones constructed with
		  their start and end points (anomalous examples will not be tested.)
		*/
		virtual bool IsContinuousLinear(const Wall&) const;
		/*********************************************************************
		*									Operator overload
		*
		* Combines this Wall object with the Wall given in the @param
		* @return the combined wall
		* Two walls can be combined only if one of the two cases below are
		  satisfied:
			- They are both straight walls such that one starts at the point
			  where the other ends
			- They are both curvy walls such that one starts at the point where
			the other ends and their center is common (coordinates of their
			  center points are the same)
		 	- On the other hand, the fact that which one of the two walls starts
			  first is not important. Similar to the fact that 3+5 = 5+3.
		* For the cases which do not satisfy above, the method should throw
		  ApplePearException which was defined in Exception.h.
		* The walls to be combined will always be the ones constructed with
		  their start and end points (anomalous examples will not be tested.)
		*/
		virtual const Wall& operator+(const Wall&) const;
		/*********************************************************************
		*									Destructor
		*
		*/
		virtual ~Wall();
		/********************************************************************/
};

#endif
