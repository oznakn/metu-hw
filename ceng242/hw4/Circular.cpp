#include "Circular.h"

Circular::Circular(int id, float radius) : MicroOrganism(id) {
	this->_radius = radius;
}

Circular::~Circular() {

}

unsigned char Circular::Type() const {
	return MicroOrganism::TYPE_CIRCULAR;
}

bool Circular::DoesFitInto(const Cell& cell) const {
	if (this->_cell != NULL) return false;

	Cell new_cell(cell);
	new_cell.StrengthenCellWall();

	const vector<Wall*> &cell_walls = new_cell.GetCellWall();

	if (cell_walls.size() != 1) return false;

	CurvyWall* curvy_wall = dynamic_cast<CurvyWall*>(cell_walls[0]);
	if (curvy_wall == NULL) return false;

	return abs(this->_radius - curvy_wall->GetRadius()) <= EPSILON;
}

void Circular::ConnectToCell(Cell* const cell) {
	cell->StrengthenCellWall();
	this->_cell = cell;

	for (int i = 0; i < this->_edges.size(); i++) {
		delete this->_edges[i];
	}

	this->_edges.clear();

	const vector<Wall*> &cell_walls = cell->GetCellWall();

	for (int i = 0; i < cell_walls.size(); i++) {
		this->_edges.push_back(cell_walls[i]->Clone());
	}
}

void Circular::React() {
	if (this->_cell != NULL) {
		this->_cell->StrengthenCellWall();

		const vector<Wall*> &cell_walls = this->_cell->GetCellWall();

		if (this->_edges.size() == 1 && cell_walls.size() == 1) {
			CurvyWall* curvy_wall = dynamic_cast<CurvyWall*>(cell_walls[0]);

			const Particle &start_particle = curvy_wall->GetInitialParticle();
			const Particle &end_particle = curvy_wall->GetFinalParticle();
			const Particle &center_particle = curvy_wall->GetCenter();

			Particle new_start_particle = Particle(
				2 * start_particle.X() - center_particle.X(),
				2 * start_particle.Y() - center_particle.Y()
			);

			Particle new_end_particle = Particle(
				2 * end_particle.X() - center_particle.X(),
				2 * end_particle.Y() - center_particle.Y()
			);

			CurvyWall* new_cell_wall = new CurvyWall(
				new_start_particle,
				new_end_particle,
				center_particle
			);

			delete this->_edges[0];
			this->_edges[0] = new_cell_wall->Clone();

			vector<Wall*> new_cell_walls;
			new_cell_walls.push_back(new_cell_wall);

			this->_cell->RenewCellWall(new_cell_walls);
		}
	}
}

bool Circular::DoesContain(const Particle& particle) const {
	if (this->_edges.size() == 1) {
		CurvyWall* curvy_wall = dynamic_cast<CurvyWall*>(this->_edges[0]);

		return curvy_wall->GetCenter().FindDistance(particle) <= curvy_wall->GetRadius();
	}

	return false;
}
