/*
#include "GUI.hpp"

int main() {
    GUI gui;
    gui.run();
    return 0;
}
*/

#include "../include/GUI.hpp"
#include <emscripten.h>

GUI* gui = nullptr;

void main_loop() {
    gui->mainLoopStep();  // You’ll need to split your mainLoop() into a step-function version
}

int main() {
    gui = new GUI();
    emscripten_set_main_loop(main_loop, 0, 1);
    return 0;
}
