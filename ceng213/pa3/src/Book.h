#ifndef BOOK_H
#define BOOK_H

#include <string>

using namespace std;

class Book {

private:
    string isbn;
    string name;
    string category;
    string writer;
    string publisher;
    int first_pub_date;
    int page_count;

public:
// Constructors
    Book() : isbn("") {}

    Book(const string isbn, const string name, const string category, const string writer, const string publisher,
         int first_pub_date, int page_count) : isbn(isbn), name(name), category(category), writer(writer),
                                                             publisher(publisher), first_pub_date(first_pub_date),
                                                             page_count(page_count) {}
// Getters and setters
    const string &getIsbn() const {
        return isbn;
    }

    void setIsbn(const string &isbn) {
        Book::isbn = isbn;
    }

    const string &getName() const {
        return name;
    }

    void setName(const string &name) {
        Book::name = name;
    }

    const string &getCategory() const {
        return category;
    }

    void setCategory(const string &category) {
        Book::category = category;
    }

    const string &getWriter() const {
        return writer;
    }

    void setWriter(const string &writer) {
        Book::writer = writer;
    }

    const string &getPublisher() const {
        return publisher;
    }

    void setPublisher(const string &publisher) {
        Book::publisher = publisher;
    }

    int getFirst_pub_date() const {
        return first_pub_date;
    }

    void setFirst_pub_date(int first_pub_date) {
        Book::first_pub_date = first_pub_date;
    }

    int getPage_count() const {
        return page_count;
    }

    void setPage_count(int page_count) {
        Book::page_count = page_count;
    }

};

#endif //BOOK_H
