#include "Wall.h"

Wall::Wall() {
	this->_start_particle = NULL;
	this->_end_particle = NULL;

	this->_length = 0;
}

Wall::Wall(float length) {
	this->_start_particle = NULL;
	this->_end_particle = NULL;

	this->_length = length;
}

Wall::Wall(const Particle& start_particle, const Particle& end_particle) {
	this->_start_particle = new Particle(start_particle);
	this->_end_particle = new Particle(end_particle);

	this->ComputeLength();
}

Wall::Wall(const Wall& rhs) {
	if (rhs._start_particle != NULL) {
		this->_start_particle = new Particle(*rhs._start_particle);
	} else {
		this->_start_particle = NULL;
	}

	if (rhs._end_particle != NULL) {
		this->_end_particle = new Particle(*rhs._end_particle);
	}else {
		this->_end_particle = NULL;
	}

	this->_length = rhs._length;
}

unsigned char Wall::Type() const {
	return Wall::TYPE_STRAIGHT;
}

float Wall::Slope() const {
	float diff_x = this->_start_particle->X() - this->_end_particle->X();
	float diff_y = this->_start_particle->Y() - this->_end_particle->Y();

	if (diff_x == 0) {
		return 9999999;
	}

	return diff_y / diff_x;
}

const Particle& Wall::GetInitialParticle() const {
	return *this->_start_particle;
}

const Particle& Wall::GetFinalParticle() const {
	return *this->_end_particle;
}

float Wall::FindAngleBetween(const Wall& rhs) const {
	return atan(this->Slope()) - atan(rhs.Slope());
}

float Wall::GetLength() const {
	return this->_length;
}

void Wall::ComputeLength() {
	this->_length = this->_start_particle->FindDistance(*this->_end_particle);
}

Wall* Wall::Clone() const {
	return new Wall(*this);
}

bool Wall::IsContinuousLinear(const Wall& rhs) const {
	return rhs.Type() == TYPE_STRAIGHT && (
			this->GetInitialParticle() == rhs.GetFinalParticle() ||
			this->GetFinalParticle() == rhs.GetInitialParticle()
		) && (abs(this->Slope() - rhs.Slope()) <= EPSILON);
}

const Wall& Wall::operator+(const Wall& rhs) const {
	if (rhs.Type() == Wall::TYPE_STRAIGHT) {
		if (this->IsContinuousLinear(rhs)) {
			if (this->GetInitialParticle() == rhs.GetFinalParticle()) {
				return *(new Wall(rhs.GetInitialParticle(), this->GetFinalParticle()));
			} else if (this->GetFinalParticle() == rhs.GetInitialParticle()) {
				return *(new Wall(this->GetInitialParticle(), rhs.GetFinalParticle()));
			}
		}
	}

	throw ApplePearException();
}


Wall::~Wall() {
	if (this->_start_particle != NULL) {
		delete this->_start_particle;
	}

	if (this->_end_particle != NULL) {
		delete this->_end_particle;
	}
}
