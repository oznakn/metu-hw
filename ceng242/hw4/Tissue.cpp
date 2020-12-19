#include "Tissue.h"

Tissue::Tissue() {
	this->_x1 = 0;
	this->_x2 = 0;
	this->_y1 = 0;
	this->_y2 = 0;
}

Tissue::Tissue(float x1, float x2, float y1, float y2) {
	this->_x1 = x1;
	this->_x2 = x2;
	this->_y1 = y1;
	this->_y2 = y2;
}

Tissue::Tissue(const Tissue& rhs) {
	this->_x1 = rhs._x1;
	this->_x2 = rhs._x2;
	this->_y1 = rhs._y1;
	this->_y2 = rhs._y2;
}

Tissue::~Tissue() {
	for (int i = 0; i < this->_cells.size(); i++) {
		delete this->_cells[i];
	}

	this->_cells.clear();
}

float* Tissue::GetCoordinates() {
	float* coordinates = new float[4];
	coordinates[0] = this->_x1;
	coordinates[1] = this->_x2;
	coordinates[2] = this->_y1;
	coordinates[3] = this->_y2;

	return coordinates;
}

void Tissue::AddCell(Cell& cell) {
	this->_cells.push_back(&cell);
}

void Tissue::RemoveCell(Cell& cell) {
	for (int i = 0; i < this->_cells.size(); i++) {
		if (this->_cells[i]->GetCellID() == cell.GetCellID()) {
			this->_cells.erase(this->_cells.begin() + i);

			break;
		}
	}
}

const Cell& Tissue::GetCell(int cell_id) const {
	for (int i = 0; i < this->_cells.size(); i++) {
		if (this->_cells[i]->GetCellID() == cell_id) {
			return *this->_cells[i];
		}
	}

	throw DeadCellException();
}

void Tissue::Interact(const vector<MicroOrganism*>& micro_organisms) {
	for (int i = 0; i < micro_organisms.size(); i++) {
		for (int j = 0; j < this->_cells.size(); j++) {
			if (micro_organisms[i]->DoesFitInto(*this->_cells[j])) {
				micro_organisms[i]->ConnectToCell(this->_cells[j]);
				break;
			}
		}
	}
}

void Tissue::CreatePartitions(const vector<MicroOrganism*>&) {}

void Tissue::TreatByPlasma(Tissue) {}
