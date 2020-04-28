#include "Tab.hpp"

// DO NOT CHANGE THIS FILE.

Tab::Tab()
{

}

Tab::Tab(std::string url, int timestamp)
{
    this->url = url;
    this->timestamp = timestamp;
}

Tab::Tab(const Tab &obj)
{
    this->url = obj.url;
    this->timestamp = obj.timestamp;
}

std::string Tab::getUrl() const
{
    return url;
}

void Tab::setUrl(std::string url)
{
    this->url = url;
}

std::string Tab::getWebSite() const
{
    std::string page = this->url;
    std::string delimiter = ".";
    std::string webPage = "";
	size_t pos = 0;

	for (int i = 0; i < 2; i++)
	{
		pos = page.find(delimiter);
		webPage = page.substr(0, pos);
		page.erase(0, pos + delimiter.length());
    }
	return webPage;
}

int Tab::getTimestamp() const
{
    return timestamp;
}

void Tab::setTimestamp(int timestamp)
{
    this->timestamp = timestamp;
}

std::ostream &operator<<(std::ostream &os, const Tab &obj)
{
    os << "Url: " << obj.url;
    os << " Timestamp: " << obj.timestamp;
    return os;
}

bool Tab::operator==(const Tab &rhs)
{
	if (rhs.url == "")
	{
		return this->timestamp == rhs.timestamp;
	}
	else if (rhs.timestamp == -1)
	{
		return this->url == rhs.url;
	}
    return (this->url == rhs.url) && (this->timestamp == rhs.timestamp);
}

bool Tab::operator<=(const Tab &rhs)
{
    return this->timestamp <= rhs.timestamp;
}
