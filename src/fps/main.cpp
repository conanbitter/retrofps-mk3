#include "rgf.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;
const int SCREEN_SCALE = 2;

int main() {
    App& app = App::getInstance();
    app.init("RetroFPS", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
    app.run();
}