#include <iostream>

#include "LinkedList.hpp"

int main() {
    LinkedList<int> llist;
    llist.insertAtTheEnd(10);

    llist.printAllNodes();

    llist.insertAtTheEnd(1);
    llist.insertAtTheEnd(7);
    llist.insertAtTheEnd(4);
    llist.printAllNodes();

    llist.removeNode(llist.findNode(7));
    llist.printAllNodes();

    llist.removeAllNodes();
    llist.insertAtTheEnd(8);
    llist.printAllNodes();

    return 0;
}
