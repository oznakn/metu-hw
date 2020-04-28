#include <iostream>
#include <cstdlib>
#include <sstream>

#include <fstream>
#include <locale.h>
#include <vector>

#include "HashTable.h"
#include "Book.h"

using namespace std;

int outof;
int score;
int current;

void SetPoints(int points) {
    current = points;
}

void Describe(const std::string& test, int points, bool critical = false) {
    outof += points;
    score += points;
    cout << test << " (" << points << ")" << endl;
    SetPoints(points);
}

template <class T>
void AssertEqual(const T& a, const T& b) {
    if (a != b) {
        cout << "Failed" << endl;
        cout << "Expected: " << b << endl;
        cout << "Output: " << a << endl;
        score -= current;
        SetPoints(0);
    }
}

void Assert(bool cond) {
    if (!cond) {
        cout << "Failed" << endl;
        score -= current;
        SetPoints(0);
    }
}

vector<string> split(string line, char delim) {
    vector<string> result;
    stringstream s_stream(line); //create string stream from the string
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, delim); //get first string delimited by comma
        result.push_back(substr);
    }
    //for(int i = 0; i<result.size(); i++) {    //print all splitted strings
    //    cout << result.at(i) << endl;
    //}
    return result;
}

vector<Book> readInput(string inputPath, char delim) {
    vector<Book> books;
    setlocale(LC_ALL, "Turkish");

    ifstream infile;
    string line;
    int lineNo = 0;

    infile.open(inputPath.c_str());
    getline(infile, line);
    while (getline(infile, line)) {
        lineNo++;
        vector<string> args = split(line, delim);
        if (args.size() == 7) {
            int pub_date = atoi(args[5].c_str());//stoi(args[5]);
            int page_cnt = atoi(args[6].c_str());//stoi(args[6]);
            Book book(args[0], args[1], args[2], args[3], args[4], pub_date ,page_cnt);
            books.push_back(book);
        }
        else {
            cout << "Not enough arguments to create a book object! at line no: " << lineNo << endl;
        }


    }
	infile.close();
    return books;
}

int main() {
    vector<Book> books = readInput("books.csv", '|');
    cout << "<|--" << endl;


    HashTable<Book> table;

    for (int i = 0; i < 64; i++) {
        table.Insert(books[i].getIsbn(), books[i]);
        table.Get(books[i].getIsbn());
    }

    Describe("Size", 3);
    AssertEqual(table.Size(), 64);

    cout << "--|>" << endl;
    cout << "Score: " << score << endl;
}