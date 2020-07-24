#include "forwardList.h"
#include "list.h"

#include <iostream>

int main() {
    List<int> l;
    l.push_front(12);
    l.push_back(13);

    l.pop_back();

    for (auto it : l) {
        std::cout << it << std::endl;
    }

    return 0;
}