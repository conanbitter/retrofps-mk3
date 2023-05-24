#include "rgf.hpp"
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;
const int SCREEN_SCALE = 2;

void Log(const char* line) {
    std::cout << line << std::endl;
}

class TestScene : public Scene {
    void onLoad(App& app) override {
        Scene::onLoad(app);
        Log("Load");
    }

    void onUnload(App& app) override {
        Scene::onUnload(app);
        Log("Unload");
    }

    void onUpdate(float deltaTime, App& app) override {
        Log("Update");
    }

    void onRender(App& app) override {
        Log("Render");
    };
};

int main() {
    TestScene scene;
    App& app = App::getInstance();
    app.init("RetroFPS", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
    app.setScene(&scene);
    app.run();
}