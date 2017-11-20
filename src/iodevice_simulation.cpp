#include "iodevice_simulation.hpp"

#include <string>
#include <sstream>
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

void IODeviceSimulation::setDim(int width, int height){

}

std::vector<std::vector<int>> IODeviceSimulation::getForeground(){

}

int IODeviceSimulation::getForeground(int x, int y){

}

std::vector<std::vector<int>> IODeviceSimulation::getBackground(){

}

int IODeviceSimulation::getBackground(int x, int y){

}

std::vector<std::vector<char>> IODeviceSimulation::getPrintedChars(){

}

int IODeviceSimulation::getPrintedChar(int x, int y){

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

}

int IODeviceSimulation::getWidth(){

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
