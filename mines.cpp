#include <iostream>

#include "controller.hpp"

#include <cstdio>

int main() {
    std::cout << "Hello World!" << std::endl;
    int key = 1337;
    Controller con(8, 8, 10, 123);
    con.print();
    while (key >= 10) {
        key = std::getchar();
        if (32 == key) {
            con.click();
        } else if (119 == key) {
            con.moveUp();
        } else if (97 == key) {
            con.moveLeft();
        } else if (115 == key) {
            con.moveDown();
        } else if (100 == key) {
            con.moveRight();
        } else if (102 == key) {
            con.tooggleFlag();
        }

        std::cout << std::endl;
        con.print();
    }
}
