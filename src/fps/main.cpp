#include "rgf.hpp"
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;
const int SCREEN_SCALE = 2;

void Log(const char* line) {
    std::cout << line << std::endl;
}

std::vector<Color> pal{
    {0, 0, 0},
    {255, 200, 100}};

class TestScene : public Scene {
    int x;
    int y;

    void onLoad(App& app, Renderer& gfx) override {
        Scene::onLoad(app, gfx);
        gfx.setPalette(pal);
        gfx.clear(0);
    }

    void onUnload(App& app, Renderer& gfx) override {
        Scene::onUnload(app, gfx);
    }

    void onUpdate(float deltaTime, App& app, Renderer& gfx) override {
        x = rand() % SCREEN_WIDTH;
        y = rand() % SCREEN_HEIGHT;
    }

    void onRender(App& app, Renderer& gfx) override {
        gfx.putPixel(x, y, 1);
    };
};

int main() {
    TestScene scene;
    App& app = App::getInstance();
    app.init("RetroFPS", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
    app.setScene(&scene);
    app.run();
}