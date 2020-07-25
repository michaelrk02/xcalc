#include <iostream>
#include <string>

#include "Calculator.h"

int main() {
    while (true) {
        std::string input;
        std::cout << "Enter mathematical expression (without spaces):" << std::endl;
        std::cout << " * ";
        std::cin >> input;

        Calculator *calc = NULL;
        try {
            calc = new Calculator(input);
            double result = calc->getResult();
            std::cout << " -> " << result << std::endl;
        } catch (const char *err) {
            std::cerr << "ERROR: " << err << std::endl;
        } catch (...) {
            std::cerr << "Unknown error occured" << std::endl;
        }
        if (calc != NULL) {
            delete calc;
        }
    }
}

