#include "NucleoBase.h"

NucleoBase::NucleoBase(string rna, NucleoBase *prev) {
	this->_prev = prev;

	if (rna.length() != 0) {
		this->_value = rna[0];

		string rest_rna = rna.substr(1, rna.length() - 1);

		if (rest_rna.length() == 0) {
			this->_next = NULL;
		} else {
			this->_next = new NucleoBase(rest_rna, this);
		}
	}
}

NucleoBase::NucleoBase(const NucleoBase& rhs) {
	if (rhs._value != 0) {
		this->_value = rhs._value;
		this->_prev = NULL;

		if (rhs._next != NULL) {
			this->_next = new NucleoBase(*rhs._next, this);
		} else {
			this->_next = NULL;
		}
	}
}

NucleoBase::NucleoBase(const NucleoBase& rhs, NucleoBase* prev) {
	if (rhs._value != 0) {
		this->_value = rhs._value;
		this->_prev = prev;

		if (rhs._next != NULL) {
			this->_next = new NucleoBase(*rhs._next, this);
		} else {
			this->_next = NULL;
		}
	}
}


NucleoBase::~NucleoBase() {
	if (this->_next != NULL) {
		delete this->_next;
	}
}

NucleoBase* NucleoBase::GetLast() {
	NucleoBase* head = this;

	while (head->_next != NULL) {
		head = head->_next;
	}

	return head;
}

ostream& operator<<(ostream& os, const NucleoBase& nucleo_base) {
	const NucleoBase *head = &nucleo_base;

	while (head != NULL && head->_value != 0) {
		os << head->_value;
		head = head->_next;
	}

	return os;
}
