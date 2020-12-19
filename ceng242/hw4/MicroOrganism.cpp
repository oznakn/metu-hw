#include "MicroOrganism.h"

MicroOrganism::MicroOrganism() {
	this->_id = -1;
	this->_cell = NULL;
}

MicroOrganism::MicroOrganism(int id) {
	this->_id = id;
    this->_cell = NULL;
}

MicroOrganism::MicroOrganism(int id, const vector<Wall*>& edges) {
	this->_id = id;
	this->_cell = NULL;

	for (int i = 0; i < edges.size(); i++) {
		this->_edges.push_back(edges[i]->Clone());
	}
}

MicroOrganism::~MicroOrganism() {
	for (int i = 0; i < this->_edges.size(); i++) {
		delete this->_edges[i];
	}
}

ostream& operator<<(ostream& os, const MicroOrganism& micro_organism) {
	if (micro_organism._cell != NULL) {
		os << "The microorganism " << micro_organism._id << " was successfully placed into the cell " << micro_organism._cell->GetCellID() << ".";
	} else {
		os << "The microorganism " << micro_organism._id << " could not be placed into any cell!";
	}

	return os;
}
