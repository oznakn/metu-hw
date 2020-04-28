#include "WebHistory.hpp"

WebHistory::WebHistory()
{
    // Does nothing.
}

WebHistory::WebHistory(std::string historyText)
{
	// history = url timestamp | url timestamp | url timestamp...
    // urls are string and timestamps are non-negative integers.
    std::string delimiter = " | ";

	std::string tabInfo;
    std::string url;
    std::string timestamp;
    size_t pos = 0;

    while (true)
	{
        pos = historyText.find(delimiter);

        bool breakTheLoop = (pos == std::string::npos);

        tabInfo = historyText.substr(0, pos);
        historyText.erase(0, pos + delimiter.length());

        pos = tabInfo.find(" ");

        url = tabInfo.substr(0, pos);
        timestamp = tabInfo.substr(pos + 1, tabInfo.length() - pos);

		Node<Tab> *newPage = new Node<Tab>(Tab(url, std::atoi(timestamp.c_str())), NULL, NULL);
        insertInOrder(newPage);

        if (breakTheLoop)
		{
			break;
		}
    }
}

void WebHistory::printHistory()
{
	std::cout << "Your web history:" << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << std::endl;
	if (history.getHead()->next == history.getTail())
	{
		std::cout << "History is empty." << std::endl;
		std::cout << std::endl;
	}
	else
	{
		Node<Tab> *node = history.getHead()->next;

		while (node != history.getTail())
		{
		    std::cout << "Page: " << node->element.getUrl() << std::endl;
			std::cout << "Last Visited: " << node->element.getTimestamp() << std::endl;
			std::cout << std::endl;
		    node = node->next;
		}
	}
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << std::endl;
}

WebHistory::WebHistory(std::string url, int timestamp)
{
	Tab tab(url, timestamp);

    this->history.insertAtTheEnd(tab);
}

WebHistory::~WebHistory()
{

}

void WebHistory::insertInOrder(Node<Tab> *newPage)
{
    Node<Tab> *start = this->history.getHead()->next;
    Node<Tab> *tail = this->history.getTail();

    while (start != NULL) {
        if (start == tail || start->element.getTimestamp() < newPage->element.getTimestamp()) {
            newPage->next = start;
            newPage->prev = start->prev;

            start->prev->next = newPage;
            start->prev = newPage;

            break;
        }

        start = start->next;
    }
}

void WebHistory::goToPage(std::string url, int timestamp)
{
    Tab tab(url, timestamp);

	Node<Tab> *node = new Node<Tab>(tab, NULL, NULL);

    this->insertInOrder(node);
}

void WebHistory::clearHistory()
{
	this->history.removeAllNodes();
}

void WebHistory::clearHistory(int timestamp)
{
	Node<Tab> *start = this->history.getHead()->next;
    Node<Tab> *tail = this->history.getTail();

    while (start != NULL && start != tail) {
        if (start->element.getTimestamp() <= timestamp) break;

        start = start->next;
    }

    while (start != NULL && start != tail) {
        this->history.removeNode(start);

        start = start->next;
    }
}

WebHistory WebHistory::operator+(const WebHistory &rhs) const
{
	WebHistory webHistory;

    Node<Tab> *start, *tail;

    start = rhs.history.getHead()->next;
    tail = rhs.history.getTail();

    while (start != NULL && start != tail) {
        webHistory.goToPage(start->element.getUrl(), start->element.getTimestamp());

        start = start->next;
    }

    start = this->history.getHead()->next;
    tail = this->history.getTail();

    while (start != NULL && start != tail) {
        webHistory.goToPage(start->element.getUrl(), start->element.getTimestamp());

        start = start->next;
    }

    return webHistory;
}

int WebHistory::timesVisited(std::string pageName)
{
    int visitedCount = 0;

    Node<Tab> *start = this->history.getHead()->next;
    Node<Tab> *tail = this->history.getTail();

    while (start != NULL && start != tail) {
        if (start->element.getWebSite() == pageName) ++visitedCount;

        start = start->next;
    }

    return visitedCount;
}

std::string WebHistory::mostVisited()
{
	LinkedList<std::string> pageNames;

    Node<Tab> *start = this->history.getHead()->next;
    Node<Tab> *tail = this->history.getTail();

    while (start != NULL && start != tail) {
        std::string pageName = start->element.getWebSite();

        if (pageNames.findNode(pageName) == NULL) {
            pageNames.insertAtTheEnd(pageName);
        }

        start = start->next;
    }

    int mostVisitedCount = 0;
    std::string mostVisited = "";

    Node<std::string> *mostVisitedStart = pageNames.getHead()->next;
    Node<std::string> *mostVisitedTail = pageNames.getTail();

    while (mostVisitedStart != NULL && mostVisitedStart != mostVisitedTail) {
        int visitedCount = this->timesVisited(mostVisitedStart->element);

        if (visitedCount > mostVisitedCount) {
            mostVisitedCount = visitedCount;
            mostVisited = mostVisitedStart->element;
        }

        mostVisitedStart = mostVisitedStart->next;
    }

    return mostVisited;
}
