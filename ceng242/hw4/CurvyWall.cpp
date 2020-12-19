#include "CurvyWall.h"

CurvyWall::CurvyWall(float length, float radius) : Wall(length) {
	this->_center_particle = NULL;

	this->_radius = radius;
}

CurvyWall::CurvyWall(const Particle& start_particle, const Particle& end_particle, const Particle& center_particle) {
	this->_start_particle = new Particle(start_particle);
	this->_end_particle = new Particle(end_particle);
	this->_center_particle = new Particle(center_particle);

	this->ComputeLength();
	this->ComputeRadius();
}

CurvyWall::CurvyWall(const CurvyWall& rhs) : Wall(rhs) {
	if (rhs._center_particle != NULL) {
		this->_center_particle = new Particle(*rhs._center_particle);
	} else {
		this->_center_particle = NULL;
	}

	this->_radius = rhs._radius;
}

unsigned char CurvyWall::Type() const {
	return Wall::TYPE_CURVY;
}

void CurvyWall::ComputeRadius() {
	this->_radius = this->_start_particle->FindDistance(*this->_center_particle);
}

float CurvyWall::GetRadius() const {
	return this->_radius;
}

const Particle& CurvyWall::GetCenter() const {
	return *this->_center_particle;
}

void CurvyWall::ComputeLength() {
	float angle_end, angle_start;
	float diff_x, diff_y;

	diff_x = this->_center_particle->X() - this->_end_particle->X();
	diff_y = this->_center_particle->Y() - this->_end_particle->Y();

	if (diff_x <= EPSILON) {
		angle_end = diff_y > 0 ? (3*PI/2) : (PI/2); // 270 : 90
	} else {
		angle_end = atan2(diff_y, diff_x);
	}

	diff_x = this->_center_particle->X() - this->_start_particle->X();
	diff_y = this->_center_particle->Y() - this->_start_particle->Y();

	if (diff_x <= EPSILON) {
		angle_start = diff_y > 0 ? (3*PI/2) : (PI/2); // 270 : 90
	} else {
		angle_start = atan2(diff_y, diff_x);
	}

	this->_length = this->_radius * (angle_end - angle_start);
}

Wall* CurvyWall::Clone() const {
	return new CurvyWall(*this);
}

bool CurvyWall::IsContinuousLinear(const Wall& rhs) const {
	return false;
}

const Wall& CurvyWall::operator+(const Wall& rhs) const {
	if (rhs.Type() == Wall::TYPE_CURVY) {
		CurvyWall curvy_wall = dynamic_cast<const CurvyWall&>(rhs);

		if (this->GetCenter() == curvy_wall.GetCenter()) {
			if (curvy_wall.GetFinalParticle() == this->GetInitialParticle()) {
				return *(new CurvyWall(curvy_wall.GetInitialParticle(), this->GetFinalParticle(), curvy_wall.GetCenter()));
			}
			else if (this->GetFinalParticle() == curvy_wall.GetInitialParticle()) {
				return *(new CurvyWall(this->GetInitialParticle(), curvy_wall.GetFinalParticle(), this->GetCenter()));
			}
		}
	}

	throw ApplePearException();
}

CurvyWall::~CurvyWall() {
	if (this->_center_particle != NULL) {
		delete this->_center_particle;
	}
}
