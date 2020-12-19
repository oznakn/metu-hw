#include "Particle.h"

Particle::Particle(float x, float y) {
	this->_x = x;
	this->_y = y;
}

Particle::Particle(const Particle& rhs) {
	this->_x = rhs._x;
	this->_y = rhs._y;
}

float Particle::X() const {
	return this->_x;
}

float Particle::Y() const {
	return this->_y;
}

float Particle::FindDistance(const Particle& rhs) const {
	return sqrt(pow(this->_x - rhs._x, 2) + pow(this->_y - rhs._y, 2));
}

bool Particle::operator==(const Particle& rhs) const {
	return abs(this->_x - rhs._x) <= EPSILON && abs(this->_y - rhs._y) <= EPSILON;
}

ostream& operator<<(ostream& os, const Particle& p) {
	os << "(" << p._x << "," << p._y << ")";

	return os;
}
