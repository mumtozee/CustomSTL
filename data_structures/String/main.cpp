#include <iostream>
#include "String.hpp"

int main() {
    String str(12, 't');
    String str2(str);
    String str3;
    str3 = str;
    String str4;
    String str5;
    String str6;
    std::cin >> str4 >> str5;
    str6 = str4 + str5;
    str.PushBack('a');
    str3.PopBack();
    str2.Resize(5);

    std::cout << str << " " << str2.Data() << " " << str3 << " "
    << str4 << " " << str5 << " " << str6 << std::endl;

    String test_str1("lorem ipsum dolor set amit");
    String test_str2(test_str1);
    test_str1 = test_str1 + " " + test_str2;
    std::cout << test_str1.Size() << " " << test_str1.Capacity() << "\n";
    test_str1.Reserve(100);
    std::cout << test_str1.Size() << " " << test_str1.Capacity() << "\n";
    test_str1.ShrinkToFit();
    std::cout << test_str1.Size() << " " << test_str1.Capacity() << "\n";

    if (test_str1 > test_str2) {
        std::cout << test_str1;
    } else if (test_str1 == test_str2) {
        std::cout << "equal";
    } else {
        std::cout << test_str2;
    }

    return 0;
}
