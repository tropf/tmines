#include "iodevice_simulation.hpp"

#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <curses.h>

void IODeviceSimulation::checkColorMode() {
    if (!colorMode) {
        throw std::runtime_error("Color mode currently disabled");
    }
} 

void IODeviceSimulation::checkWindowActive() {
    if (!windowActive) {
        throw std::runtime_error("current window is not active. call initwindow!");
    }
}

int IODeviceSimulation::getChar() {
    int c;
    while (1 > input.size()) {
        // busy wait
    }
    c = input.front();
    input.pop_front();

    // KEY_RESIZE
    if (mockRemaining > 0 && KEY_RESIZE == c) {
        mockRemaining = 0;
    } 
    return c;
}

void IODeviceSimulation::addChar(int nextchar){
    input.push_back(nextchar);
}

void IODeviceSimulation::addChars(std::string nextchars){
    for (int character : nextchars) {
        input.push_back(character);
    }
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

int IODeviceSimulation::getCursorX() {
    return cursorX;
}

int IODeviceSimulation::getCursorY() {
    return cursorY;
}

int IODeviceSimulation::getCursorVisibility() {
    return cursorVisibility;
}

int IODeviceSimulation::getClearCount() {
    return clearCnt;
}

int IODeviceSimulation::getRefreshCount() {
    return refreshCnt;
}

int IODeviceSimulation::getInitCount() {
    return initCnt;
}

int IODeviceSimulation::getEndWindowCount() {
    return endWinCnt;
}

bool IODeviceSimulation::isEchoMode() {
    return echoMode;
}

bool IODeviceSimulation::isColorStarted() {
    return colorMode;
}

bool IODeviceSimulation::isSpecialKeysEnabled() {
    return specialKeysEnabled;
}

std::map<int, std::tuple<int, int>> IODeviceSimulation::getColorPairs() {
    return colors;
}


void IODeviceSimulation::setColor(int colorCode){
    checkColorMode();
    // check if colorCode even exists
    if (colors.find(colorCode) == colors.end()) {
        throw std::runtime_error("given color code is unknown");
    }
    currentColor = colorCode;
}

void IODeviceSimulation::putString(int x, int y, std::string to_print){
    checkWindowActive();
    checkPos(x, y);

    // check if window is wide enough
    if (x + to_print.length() > getWidth()) {
        throw std::runtime_error("given string cannot be printed at given pos, width too small");
    }

    // all checks good, print
    int fg, bg;
    std::tie(fg, bg) = colors[currentColor];

    for (int i = 0; i < to_print.length(); i++) {
        foreground[x + i][y] = fg;
        background[x + i][y] = bg;
        chars[x + i][y] = to_print[i];
    }
}

void IODeviceSimulation::moveCursor(int x, int y){
    checkWindowActive();
    checkPos(x, y);
    cursorX = x;
    cursorY = y;
}

void IODeviceSimulation::setCursorVisibility(int visibility){
    checkWindowActive();
    // only valid values: 0, 1, 2
    if (visibility < 0 || visibility > 2) {
        throw std::runtime_error("Given visibilty must be either 0, 1, 2");
    }
    cursorVisibility = visibility;
}

int IODeviceSimulation::getHeight(){
    checkWindowActive();
    if (0 == mockRemaining) {
        return 1;
    }

    if (getWidth() > 0) {
        return chars[0].size();
    } else {
        return 0;
    }
}

int IODeviceSimulation::getWidth(){
    checkWindowActive();
    if (0 == mockRemaining) {
        return 1;
    }
    return chars.size();
}

void IODeviceSimulation::clear(){
    checkWindowActive();
    auto width = getWidth();
    auto height = getHeight();
    chars.clear();
    foreground.clear();
    background.clear();

    setDim(width, height);

    clearCnt++;
}

void IODeviceSimulation::refresh(){
    checkWindowActive();
    refreshCnt++;
}

void IODeviceSimulation::initWindow(){
    windowActive = true;
    initCnt++;
}

void IODeviceSimulation::endWindow(){
    windowActive = false;
    endWinCnt++;
}

void IODeviceSimulation::setEcho(bool enabled){
    checkWindowActive();
    echoMode = enabled;
}

void IODeviceSimulation::startColor(){
    colorMode = true;
}
void IODeviceSimulation::startSpecialKeys(){
    checkWindowActive();
    specialKeysEnabled = true;
}

void IODeviceSimulation::addColor(int id, int fg, int bg){
    checkColorMode();
    
    auto pair = std::make_tuple(fg, bg);
    colors[id] = pair;
}

void IODeviceSimulation::setBackground(int id){
    checkColorMode();
    checkWindowActive();

    // check if colorCode even exists
    if (colors.find(id) == colors.end()) {
        throw std::runtime_error("given color code is unknown");
    }
    
    int fg, bg;
    std::tie(fg, bg) = colors[id];
    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            if (0 == chars[x][y]) {
                foreground[x][y] = fg;
                background[x][y] = bg;
            }
        }
    }
}

void IODeviceSimulation::mockResize(int remainder) {
    mockRemaining = remainder;
}
