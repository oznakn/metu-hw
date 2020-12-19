#include "SoftCopy.h"
/* YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. DO NOT MODIFY THE HEADER FILE AS IT WILL BE
REPLACED BY THE ORIGINAL HEADER DURING TESTING. THE REQUIRED
HEADER FILE IS ALREADY INCLUDED FOR YOU ABOVE THIS COMMENT BLOCK.
THE CONSTRUCTOR and DESTRUCTOR ARE ALREADY IMPLEMENTED.
START YOUR IMPLEMENTATIONS BELOW THOSE. */


// DONE
SoftCopy::SoftCopy(int ISBN, double price, int startPage, int endPage) : Book(ISBN, price) {
	this->startPage = startPage;
	this->endPage = endPage;
	this->numberOfPages = (endPage - startPage) + 1;
	this->firstHalf = NULL;
	this->secondHalf = NULL;
	this->pageContent = "";
}

// DONE
SoftCopy::~SoftCopy() {
	if (firstHalf != NULL)
		delete firstHalf;
	if (secondHalf != NULL)
		delete secondHalf;
}


SoftCopy::SoftCopy(const SoftCopy& rhs) : Book(rhs.ISBN, rhs.price) {
    this->startPage = rhs.startPage;
	this->endPage = rhs.endPage;
	this->numberOfPages = (rhs.endPage - rhs.startPage) + 1;

	if (rhs.firstHalf != NULL) {
	    this->firstHalf = new SoftCopy(*rhs.firstHalf);
	} else {
	    this->firstHalf = NULL;
	}

	if (rhs.secondHalf != NULL) {
	    this->secondHalf = new SoftCopy(*rhs.secondHalf);
	} else {
	    this->secondHalf = NULL;
	}

	this->pageContent = rhs.pageContent;
}

SoftCopy& SoftCopy::operator=(const SoftCopy& rhs) {
    if (&rhs == this) {
        return *this;
    }

    if (firstHalf != NULL) {
        delete firstHalf;
    }

	if (secondHalf != NULL) {
	    delete secondHalf;
	}

	this->ISBN = rhs.ISBN;
	this->price = rhs.price;
	this->next = NULL;

	this->startPage = rhs.startPage;
	this->endPage = rhs.endPage;
	this->numberOfPages = (rhs.endPage - rhs.startPage) + 1;

	if (rhs.firstHalf != NULL) {
	    this->firstHalf = new SoftCopy(*rhs.firstHalf);
	} else {
	    this->firstHalf = NULL;
	}

	if (rhs.secondHalf != NULL) {
	    this->secondHalf = new SoftCopy(*rhs.secondHalf);
	} else {
	    this->secondHalf = NULL;
	}

	this->pageContent = rhs.pageContent;

	return *this;
}

SoftCopy& SoftCopy::operator+(const SoftCopy& rhs) const {
    int startPage = this->startPage < rhs.startPage ? this->startPage : rhs.startPage;
    int endPage = this->endPage > rhs.endPage ? this->endPage : rhs.endPage;

    SoftCopy *new_book = new SoftCopy(this->ISBN, this->price, startPage, endPage);

    if (this->startPage == this->endPage) {
        new_book->UploadPage(this->pageContent, this->startPage);
    }
    if (rhs.startPage == rhs.endPage) {
        new_book->UploadPage(rhs.pageContent, rhs.startPage);
    }

    vector<SoftCopy*> queue;
    queue.push_back(this->firstHalf);
    queue.push_back(this->secondHalf);
    queue.push_back(rhs.firstHalf);
    queue.push_back(rhs.secondHalf);

    while (!queue.empty()) {
        SoftCopy *head = queue.back();
        queue.pop_back();

        if (head != NULL) {
            if (head->startPage == head->endPage) {
                new_book->UploadPage(head->pageContent, head->startPage);
            }

            queue.push_back(head->firstHalf);
            queue.push_back(head->secondHalf);
        }
    }

    return *new_book;
}

Book* SoftCopy::Clone() const {
    SoftCopy *new_book = new SoftCopy(this->ISBN, this->price, this->startPage, this->endPage);

	if (this->firstHalf != NULL) {
	    new_book->firstHalf = new SoftCopy(*this->firstHalf);
	} else {
	    new_book->firstHalf = NULL;
	}

	if (this->secondHalf != NULL) {
	    new_book->secondHalf = new SoftCopy(*this->secondHalf);
	} else {
	    new_book->secondHalf = NULL;
	}

	new_book->pageContent = this->pageContent;

	return new_book;
}

void SoftCopy::Discount() {
    this->price *= 0.75;

    vector<SoftCopy*> queue;
    queue.push_back(this->firstHalf);
    queue.push_back(this->secondHalf);

    while (!queue.empty()) {
        SoftCopy *head = queue.back();
        queue.pop_back();

        if (head != NULL) {
            head->price = this->price;

            queue.push_back(head->firstHalf);
            queue.push_back(head->secondHalf);
        }
    }
}

void SoftCopy::UploadPage(string content, int page) {
    if (this->startPage == this->endPage && this->startPage == page) {
        this->pageContent = content;
    } else {
        int mid = (this->startPage + this->endPage) / 2;

        if (page <= mid) {
            if (this->firstHalf == NULL) {
                this->firstHalf = new SoftCopy(this->ISBN, this->price, this->startPage, mid);
            }

            this->firstHalf->UploadPage(content, page);
        } else if(page >= mid + 1) {
            if (this->secondHalf == NULL) {
                this->secondHalf = new SoftCopy(this->ISBN, this->price, mid + 1, this->endPage);
            }

            this->secondHalf->UploadPage(content, page);
        }
    }
}

string SoftCopy::Display(int from, int to) const {
    vector<string> results;

    if (this->startPage == this->endPage && this->startPage >= from && this->startPage <= to) {
        results.push_back(this->pageContent);
    }

    vector<SoftCopy*> queue;
    queue.push_back(this->firstHalf);
    queue.push_back(this->secondHalf);

    while (!queue.empty()) {
        SoftCopy *head = queue.back();
        queue.pop_back();

        if (head != NULL) {
            if (head->startPage == head->endPage && head->startPage >= from && head->startPage <= to) {
                results.push_back(head->pageContent);
            }

            queue.push_back(head->firstHalf);
            queue.push_back(head->secondHalf);
        }
    }

    string result = "";

    for (int i = results.size() - 1; i >= 0; i--) {
        result += results[i];

         if (i != 0) {
            result += "\n";
        }
    }

    return result;
}