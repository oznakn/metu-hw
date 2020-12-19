/*************************************************************************
 *						DO NOT MODIFY THIS FILE !!!						 *
/************************************************************************/
#include "BookStore.h"

// DONE
BookStore::BookStore(string name) {
	this->name = name;
	initialBook = NULL;
}

// DONE
BookStore::BookStore(const BookStore& bookstore) {
	this->name = bookstore.name;
	this->initialBook = bookstore.initialBook->Clone();

	Book* nextBook = this->initialBook->GetNext();
	while (nextBook != NULL) {
		nextBook = nextBook->Clone();	// clone itself to implement deep copy
		nextBook = nextBook->GetNext();
	}
}

// DONE
Book& BookStore::GetBookList(int from, int to) {
	Book* currentBook = initialBook;
	Book* newBook, * initialQueryBook;
	int increment = 0;

	while (currentBook != NULL) {
		if (*currentBook < from)
			currentBook = currentBook->GetNext();
		else if (*currentBook > to) {
			newBook->SetNext();
			break;
		}
		else {
			if (increment == 0) {
				newBook = currentBook->Clone();
				initialQueryBook = newBook;
			}
			else {
				Book* temp =currentBook->Clone();
				newBook->SetNext(*temp);	// it creates a new one
				delete temp;				// so delete
				newBook = newBook->GetNext();
			}
			currentBook = currentBook->GetNext();
			increment++;
		}
	}
	return *initialQueryBook;
}

// DONE
void BookStore::AddNewBook(const Book& book) {
	Book* newBook = book.Clone();
	if (initialBook == NULL)
		initialBook = newBook;
	else {
		Book* currentBook = initialBook;
		Book* prev = NULL;
		while (*currentBook < book) {
			prev = currentBook;
			currentBook = currentBook->GetNext();
			if (currentBook == NULL) {
				prev->SetNext(*newBook);	// actually a new book *newBook was attached without breaking the chain
				delete newBook;				// so delete
				return;
			}
		}
		if (prev == NULL)
			initialBook = newBook;
		else {
			prev->SetNext(*newBook);		// actually a new book *newBook was attached without breaking the chain
			delete newBook;					// so delete
			newBook = prev->GetNext();
		}
		newBook->SetNext(*currentBook);		// actually a new book *currentBook was attached without breaking the chain
		delete currentBook;					// so delete
	}
}

// DONE
BookStore::~BookStore() {
	Book* currentBook = initialBook;
	while(currentBook != NULL) {
		Book* temp = currentBook->GetNext();
		delete currentBook;
		currentBook = temp;
	}
}
