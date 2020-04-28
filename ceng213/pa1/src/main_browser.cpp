#include <iostream>

#include "WebHistory.hpp"

int main() {
    WebHistory p1("www.youtube.com 4 | www.ceng.metu.edu.tr 8 | www.youtube.com/watch?v=4lYDblvvxE4 5 | https://www.youtube.com/watch?v=0lR1tt24mo4 9 | www.ntv.com.tr 10 | https://www.instagram.com/megadeth 1");

    WebHistory p2("www.instagram.com 1 | www.facebook.com 7 | https://www.youtube.com/watch?v=s9pZxnJAwQI 4 | https://www.youtube.com/watch?v=-gDinVAmtA0 8 | www.ntv.com.tr 9 | https://www.instagram.com/pinkfloyd/ 99");

    std::cout << p1.mostVisited() << std::endl;
    std::cout << std::endl;
    p1.printHistory();
    std::cout << std::endl;
    p2.printHistory();

	p1.goToPage("https://www.youtube.com/watch?v=7v8zxoEoA_Q", 91);
	p2.goToPage("https://www.youtube.com/watch?v=dhpwBPuWyKg", 6);

	p1.printHistory();
    std::cout << std::endl;
    p2.printHistory();

    p1.clearHistory();
	p2.clearHistory();

	p1.printHistory();
    std::cout << std::endl;
    p2.printHistory();

    return 0;
}
