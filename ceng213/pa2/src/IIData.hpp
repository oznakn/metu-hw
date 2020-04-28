#ifndef IIDATA_HPP
#define IIDATA_HPP

#include <iostream>
#include <vector>

// DO NOT CHANGE THIS FILE.

class IIData {
public:
    IIData();
    IIData(const std::string &word);

    std::string &getWord();
    std::vector<std::pair<std::string, std::vector<int> > > &getOccurrences();

    void setWord(const std::string &word);
    void setOccurrences(const std::vector<std::pair<std::string, std::vector<int> > > &occurrences);

    void addOccurrence(const std::string &documentName, int position);
    void removeOccurrences(const std::string &documentName);

    bool operator<(const IIData &rhs) const;
    bool operator>(const IIData &rhs) const;
    bool operator==(const IIData &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const IIData &data);

private:
    std::string word;
    std::vector<std::pair<std::string, std::vector<int> > > occurrences;
};

#endif //IIDATA_HPP
