#include "iodevice_simulation.hpp"

#include <string>
#include <sstream>
#include <stdexcept>
#include <stdexcept>

char IODeviceSimulation::getChar() {
    char c = -1;
    while (-1 == c) {
        c = input.get();
    }
    return c;
}

void IODeviceSimulation::addChar(char nextchar){
    input << nextchar; 
}

void IODeviceSimulation::addChars(std::string nextchars){
    input << nextchars;
}

void IODeviceSimulation::checkPos(int x, int y) {
    if (! isPosValid(x, y)) {
        throw std::runtime_error("Given position is invalid!");
    }
}

bool IODeviceSimulation::isPosValid(int x, int y) {
    if (x < 0 || y < 0) {
        return false;
    }

    if (x >= getWidth() || y >= getHeight()) {
        return false;
    }

    return true;
}

void IODeviceSimulation::setDim(int width, int height){
    if (0 == width || 0 == height) {
        throw std::runtime_error("width and height must be >0");
    }
    chars.resize(width);
    foreground.resize(width);
    background.resize(width);
    for (int x = 0; x < width; x++) {
        chars[x].resize(height);
        foreground[x].resize(height);
        background[x].resize(height);
    }
}

std::vector<std::vector<int>> IODeviceSimulation::getForeground(){
    return foreground;
}

int IODeviceSimulation::getForeground(int x, int y){
    checkPos(x, y);
    return foreground[x][y];
}

std::vector<std::vector<int>> IODeviceSimulation::getBackground(){
    return background;
}

int IODeviceSimulation::getBackground(int x, int y){
    checkPos(x, y);
    return background[x][y];
}

std::vector<std::vector<char>> IODeviceSimulation::getPrintedChars(){
    return chars;
}

int IODeviceSimulation::getPrintedChar(int x, int y){
    checkPos(x, y);
    return chars[x][y];
}

void IODeviceSimulation::setColor(int colorCode){

}

void IODeviceSimulation::putString(int x, int y, std::string to_print){

}

void IODeviceSimulation::moveCursor(int x, int y){

}

void IODeviceSimulation::setCursorVisibility(int visibility){

}

int IODeviceSimulation::getHeight(){
    if (getWidth() > 0) {
        return chars[0].size();
    } else {
        return 0;
    }
}

int IODeviceSimulation::getWidth(){
    return chars.size();
}

void IODeviceSimulation::clear(){

}

void IODeviceSimulation::refresh(){

}

void IODeviceSimulation::initWindow(){

}

void IODeviceSimulation::endWindow(){

}

void IODeviceSimulation::setEcho(bool enabled){

}

void IODeviceSimulation::startColor(){

}

void IODeviceSimulation::startSpecialKeys(){

}

void IODeviceSimulation::addColor(int id, int fg, int bg){

}

void IODeviceSimulation::setBackground(int id){

}
