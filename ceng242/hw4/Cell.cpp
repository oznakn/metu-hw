#include "Cell.h"

Cell::Cell(int id, const vector<Wall*>& walls, Tissue* tissue) {
	this->_id = id;

	for (int i = 0; i < walls.size(); i++) {
		this->_walls.push_back(walls[i]->Clone());
	}

	this->_tissue = tissue;
}

Cell::Cell(const Cell& rhs) {
	this->_id = rhs._id;

	for (int i = 0; i < rhs._walls.size(); i++) {
		this->_walls.push_back(rhs._walls[i]->Clone());
	}

	this->_tissue = rhs._tissue;
}

int Cell::GetCellID() const {
	return this->_id;
}

Tissue* Cell::GetTissue() const {
	return this->_tissue;
}

const vector<Wall*>& Cell::GetCellWall() const {
	return this->_walls;
}

void Cell::RenewCellWall(vector<Wall*>& walls) {
	for (int i = 0; i < this->_walls.size(); i++) {
		delete this->_walls[i];
	}

	this->_walls = walls;
}

void Cell::StrengthenCellWall() {
	if (this->_walls.size() > 0) {
		vector<Wall*> walls;

		for (int i = 0; i < this->_walls.size(); i++) {
			Wall* current_wall = this->_walls[i];

			if (i == 0) {
				walls.push_back(current_wall->Clone());
			} else {
				Wall* last_wall = walls.at(walls.size() - 1);

				try {
					const Wall *tmp = &(*last_wall + *current_wall);

					delete last_wall;
					walls.pop_back();
					walls.push_back(tmp->Clone());
					delete tmp;
				}
				catch (ApplePearException e) {
					walls.push_back(current_wall->Clone());
				}
			}
		}

		if (walls.size() >= 2) {
			Wall* first_wall = walls.at(0);
			Wall* last_wall = walls.at(walls.size() - 1);

			try {
				const Wall* tmp = &(*first_wall + *last_wall);

				delete first_wall;
				delete last_wall;
				walls.pop_back();
				walls[0] = tmp->Clone();
				delete tmp;
			}
			catch (ApplePearException e) {}
		}

		for (int i = 0; i < this->_walls.size(); i++) {
			delete this->_walls[i];
		}

		this->_walls = walls;
	}
}

Cell::~Cell() {
	for (int i = 0; i < this->_walls.size(); i++) {
		delete this->_walls[i];
	}
}

ostream& operator<<(ostream& os, const Cell& cell) {
	for (int i = 0; i < cell._walls.size(); i++) {
		if (i != 0) {
			os << " - ";
		}

		os << cell._walls[i]->GetInitialParticle();
	}

	return os;
}
