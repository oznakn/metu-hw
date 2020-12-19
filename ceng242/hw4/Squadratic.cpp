#include "Squadratic.h"

Squadratic::Squadratic(int id, float edge_length) : MicroOrganism(id) {
	this->_edge_length = edge_length;
}

Squadratic::Squadratic(const Squadratic& rhs) {
	this->_id = rhs._id;
	this->_cell = rhs._cell;

	for (int i = 0; i < rhs._edges.size(); i++) {
		this->_edges.push_back(rhs._edges[i]->Clone());
	}

	this->_edge_length = rhs._edge_length;
}

Squadratic::~Squadratic() {
	for (int i = 0; i < this->_childs.size(); i++) {
		if (this->_childs[i] != this) {
			delete this->_childs[i];
		}
	}
}

unsigned char Squadratic::Type() const {
	return MicroOrganism::TYPE_SQUADRATIC;
}

bool Squadratic::DoesFitInto(const Cell& cell) const {
	if (this->_cell != NULL) return false;

	Cell new_cell(cell);
	new_cell.StrengthenCellWall();

	const vector<Wall*> &cell_walls = new_cell.GetCellWall();

	if (cell_walls.size() != 4) return false;

	for (int i = 0; i < cell_walls.size(); i++) {
		CurvyWall* wall = dynamic_cast<CurvyWall*>(cell_walls[i]);
		if (wall != NULL) return false;
	}

	for (int i = 0; i < cell_walls.size() ; i++) {
		int next_index = (i + 1) % cell_walls.size();

		float angle = cell_walls[i]->FindAngleBetween(*cell_walls[next_index]);

		if (angle - (PI / 2) > EPSILON && angle - (-PI / 2) > EPSILON) return false;
	}

	return abs(this->_edge_length - cell_walls[0]->GetLength()) <= EPSILON;
}

void Squadratic::ConnectToCell(Cell* const cell) {
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

void Squadratic::React() {
	Cell *cell = this->_cell;
	float new_edge_length = this->_edge_length;

	vector<Particle> corners;
	vector<Particle> middles;
	float sum_x = 0;
	float sum_y = 0;

	for (int i = 0; i < 4; i++) {
		Particle particle = this->_edges[i]->GetInitialParticle();
		corners.push_back(particle);

		sum_x += particle.X();
		sum_y += particle.Y();
	}

	for (int i = 0; i < 4; i++) {
		int next_index = (i + 1) % 4;

		Particle new_middle_particle(
				(corners[i].X() + corners[next_index].X()) / 2,
				(corners[i].Y() + corners[next_index].Y()) / 2
		);

		middles.push_back(new_middle_particle);
	}

	Particle center(sum_x / 4, sum_y / 4);

	for (int i = 0; i < 4; i++) {
		int prev_index = (i - 1 + 4) % 4;

		vector<Wall*> walls;
		walls.push_back(new Wall(corners[i], middles[i]));
		walls.push_back(new Wall(middles[i], center));
		walls.push_back(new Wall(center, middles[prev_index]));
		walls.push_back(new Wall(middles[prev_index], corners[i]));

		if (i == 0) {
			for (int i = 0; i < this->_edges.size(); i++) {
				delete this->_edges[i];
			}

			this->_edges = walls;

			this->_childs.push_back(this);
		} else {
			Squadratic *new_squadratic = new Squadratic(100001, new_edge_length);

			new_squadratic->_edges = walls;
			new_squadratic->_cell = cell;

			this->_childs.push_back(new_squadratic);
		}
	}
}

Squadratic& Squadratic::GetChild(float x1, float x2, float y1, float y2) const {
	for (int i = 0; i < this->_childs.size(); i++) {
		bool is_in_cell = true;

		for (int j = 0; j < this->_childs[i]->_edges.size(); j++) {
			Wall *wall = this->_childs[i]->_edges[j];

			const Particle &start_particle = wall->GetInitialParticle();
			const Particle &end_particle = wall->GetFinalParticle();

			if (!(x1 <= start_particle.X() && start_particle.X() <= x2 &&
				y1 <= start_particle.Y() && start_particle.Y() <= y2 &&
				x1 <= end_particle.X() && end_particle.X() <= x2 &&
				y1 <= end_particle.Y() && end_particle.Y() <= y2)) {
					is_in_cell = false;
					break;
				}
		}

		if (is_in_cell) {
			return *this->_childs[i];
		}
	}

	throw NotBornChildException();
}
