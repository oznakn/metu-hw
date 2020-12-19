#include "Triangular.h"

Triangular::Triangular(int id, const vector<Wall*>& edges, const NucleoBase& nucleo_base) : MicroOrganism(id, edges) {
		this->_nucleo_base = new NucleoBase(nucleo_base);
		this->_cell = NULL;
	}

Triangular::~Triangular() {
	if (this->_nucleo_base != NULL) {
		delete this->_nucleo_base;
	}
}

unsigned char Triangular::Type() const {
	return MicroOrganism::TYPE_TRIANGULAR;
}

bool Triangular::DoesFitInto(const Cell& cell) const {
	if (this->_cell != NULL) return false;

	Cell new_cell(cell);
	new_cell.StrengthenCellWall();

	const vector<Wall*> cell_walls = new_cell.GetCellWall();

	if (cell_walls.size() != 3) return false;

	for (int i = 0; i < 3; i++) {
		CurvyWall* wall = dynamic_cast<CurvyWall*>(cell_walls[i]);
		if (wall != NULL) return false;
	}

	bool flags[3] = {false, false, false};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (abs(cell_walls[j]->GetLength() - this->_edges[i]->GetLength()) <= EPSILON &&
					!flags[j]) {
				flags[j] = true;
				break;
			}
		}
	}

	return flags[0] && flags[1] && flags[2];
}

void Triangular::ConnectToCell(Cell* const cell) {
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

void Triangular::React() {
	if (this->_cell != NULL) {
		if (this->_cell->GetTissue() != NULL) {
			this->_cell->GetTissue()->RemoveCell(*this->_cell);
		}

		delete this->_cell;
	}
}

const NucleoBase& Triangular::GetRNA() const {
	return *this->_nucleo_base;
}

void Triangular::Mutate(Triangular& triangular) {
	NucleoBase *start1 = this->_nucleo_base;
	NucleoBase *start2 = triangular._nucleo_base;

	NucleoBase *head1, *head2, *tail1, *tail2;

	head1 = this->_nucleo_base;
	head2 = triangular._nucleo_base;
	while (head1 != NULL && head2 != NULL && head1->_value == head2->_value) {
		head1 = head1->_next;
		head2 = head2->_next;
	}

	tail1 = this->_nucleo_base->GetLast();
	tail2 = triangular._nucleo_base->GetLast();
	while (tail1 != NULL && tail2 != NULL && tail1->_value == tail2->_value) {
		tail1 = tail1->_prev;
		tail2 = tail2->_prev;
	}
	tail1 = tail1->_next;
	tail2 = tail2->_next;

	while (head1 != NULL && head1 != tail1 && head2 != NULL && head2 != tail2) {
		if (head1->_value == 'A' && head2->_value == 'U') {
			head1->_value = 'U';
			head2->_value = 'A';
		} else if (head1->_value == 'U' && head2->_value == 'A') {
			head1->_value = 'A';
			head2->_value = 'U';
		} else if (head1->_value == 'G' && head2->_value == 'S') {
			head1->_value = 'S';
			head2->_value = 'G';
		} else if (head1->_value == 'S' && head2->_value == 'G') {
			head1->_value = 'G';
			head2->_value = 'S';
		} else {
			NucleoBase *tmp;

			tmp = head1->_next;

			if (head1->_prev != NULL) {
				head1->_prev->_next = head1->_next;
			}
			if (head1->_next != NULL) {
				head1->_next->_prev = head1->_prev;
			}

			if (head1 == start1) {
				start1 = head1->_next;
			}

			head1->_prev = NULL;
			head1->_next = NULL;
			delete head1;
			head1 = tmp;

			tmp = head2->_next;

			if (head2->_prev != NULL) {
				head2->_prev->_next = head2->_next;
			}
			if (head2->_next != NULL) {
				head2->_next->_prev = head2->_prev;
			}

			if (head2 == start2) {
				start2 = head2->_next;
			}

			head2->_prev = NULL;
			head2->_next = NULL;
			delete head2;
			head2 = tmp;

			continue;
		}

		head1 = head1->_next;
		head2 = head2->_next;
	}

	this->_nucleo_base = start1;
	triangular._nucleo_base = start2;
}
