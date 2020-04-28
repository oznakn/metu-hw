#ifndef TAB_HPP
#define TAB_HPP

#include <ostream>
#include<cstdlib>

// DO NOT CHANGE THIS FILE.

class Tab {
	public:
		Tab();
		Tab(std::string url, int timestamp);
		Tab(const Tab &obj);

		std::string getUrl() const;
		std::string getWebSite() const;
		void setUrl(std::string url);

		int getTimestamp() const;
		void setTimestamp(int timestamp);

		friend std::ostream &operator<<(std::ostream &os, const Tab &obj);

		bool operator==(const Tab &rhs);
		bool operator<=(const Tab &rhs);

	private:
		std::string url;
		int timestamp;
};

#endif //TAB_HPP
