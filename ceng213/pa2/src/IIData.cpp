#include "IIData.hpp"

IIData::IIData() {

}

IIData::IIData(const std::string &word) {

    this->word = word;
}

std::string &IIData::getWord() {

    return word;
}

std::vector<std::pair<std::string, std::vector<int> > > &IIData::getOccurrences() {

    return occurrences;
}

void IIData::setWord(const std::string &word) {

    this->word = word;
}

void IIData::setOccurrences(const std::vector<std::pair<std::string, std::vector<int> > > &occurrences) {

    this->occurrences = occurrences;
}

void IIData::addOccurrence(const std::string &documentName, int position) {
    bool isInVector = false;

    for (long unsigned int i = 0; i < this->occurrences.size(); i++) {
        if (this->occurrences[i].first == documentName) {
            this->occurrences[i].second.push_back(position);
            isInVector = true;

            break;
        }
    }

    if (!isInVector) {
        std::pair<std::string, std::vector<int> > new_pair;
        new_pair.first = documentName;
        new_pair.second.push_back(position);

        this->occurrences.push_back(new_pair);
    }
}

void IIData::removeOccurrences(const std::string &documentName) {
    for (long unsigned int i = 0; i < this->occurrences.size(); i++) {
        if (this->occurrences[i].first == documentName) {
            this->occurrences.erase(this->occurrences.begin() + i);

            break;
        }
    }
}

bool IIData::operator<(const IIData &rhs) const {

    return word < rhs.word;
}

bool IIData::operator>(const IIData &rhs) const {

    return word > rhs.word;
}

bool IIData::operator==(const IIData &rhs) const {

    return word == rhs.word;
}

std::ostream &operator<<(std::ostream &os, const IIData &data) {

    os << "{";
    os << "word: " << data.word << ", ";
    os << "occurrences: [";
    for (unsigned int i = 0 ; i < data.occurrences.size() ; ++i) {
        os << "(";
        os << data.occurrences[i].first << ", ";
        os << "[";
        if (!data.occurrences[i].second.empty()) {
            for (unsigned int j = 0 ; j < data.occurrences[i].second.size() - 1; ++j) {
                os << data.occurrences[i].second[j] << ", ";
            }
            os << data.occurrences[i].second[data.occurrences[i].second.size() - 1];
        }
        os << "]";
        os << ")";
        if (i != data.occurrences.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    os << "}";

    return os;
}
