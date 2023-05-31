#include "rgf.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include <utility>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;
const int SCREEN_SCALE = 2;

class EffectScene : public Scene {
    void onLoad(App& app, Renderer& gfx) override {
        Scene::onLoad(app, gfx);
    }

    void onUnload(App& app, Renderer& gfx) override {
        Scene::onUnload(app, gfx);
    }

    void onUpdate(float deltaTime, App& app, Renderer& gfx) override {
    }

    void onRender(App& app, Renderer& gfx) override{};
};

int main() {
    EffectScene scene;
    App& app = App::getInstance();
    app.init("Pixel Effect", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
    app.setScene(&scene);
    app.run();
}