#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
    short int x;
    std::cin >> x;

    if (1)
        if (x != 0)
            std::cout << 10 / x;
        else
            std::cout << "error" << std::endl;
}