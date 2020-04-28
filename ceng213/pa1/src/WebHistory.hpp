#ifndef WEBHISTORY_HPP
#define WEBHISTORY_HPP

#include <iostream>
#include <cstdlib>

#include "LinkedList.hpp"
#include "Tab.hpp"

// DO NOT CHANGE THIS FILE.

class WebHistory
{
	public:
		// GIVEN: Default Constructor
		WebHistory();
		// GIVEN: Construtor that takes series of urls and timestamps.
		// You should implement insertInOrder function to run this function.
		WebHistory(std::string history);

		// TODO: Consturctor with a single tab. The tab information is given as a url and a timestamp.
		WebHistory(std::string url, int timestamp);

		// TODO: Destructor.
		~WebHistory();

		// GIVEN: Prints the history in a specific format.
		void printHistory();

		// TODO: Insert the given tab to list in decreasing order.
		void insertInOrder(Node<Tab> *newPage);

		// TODO: Insert new tab with given url and timestamp
		void goToPage(std::string url, int timestamp);

		// TODO: Clear all tabs from the history
		void clearHistory();

		// TODO: Clear pages whose timestamp is less than or equal to the given timestamp
		void clearHistory(int timestamp);

		// TODO: Sync two histories. Do not forget to check the timestamps while merging.
		// Resulting list should still be in deacrasing order of timestamps.
		WebHistory operator+(const WebHistory &rhs) const;

		//BONUS PARTS

		// Bonus: Return how many times the given pageName is visited.
		// Hint: Use a function given in Tab class to strip the url.
		int timesVisited(std::string pageName);
		// Bonus: Return the most visited page. Do not use the full links.
		// (i.e. if link contains www.youtube.com/etcetc it should be counted as youtube.
		// Use a function given in Tab class to strip the url.)
		std::string mostVisited();

	private:
		LinkedList<Tab> history;
};

#endif //WEBHISTORY_HPP
