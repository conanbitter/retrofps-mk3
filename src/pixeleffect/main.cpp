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
   public:
    EffectScene() : back(SCREEN_WIDTH + 2, SCREEN_HEIGHT + 2) {}

    void onLoad(App& app, Renderer& gfx) override {
        Scene::onLoad(app, gfx);
        for (int i = 0; i < 256; i++) {
            gfx.setColor(i, Color(i, i, i));
        }
    }

    void onUnload(App& app, Renderer& gfx) override {
        Scene::onUnload(app, gfx);
    }

    void onUpdate(float deltaTime, App& app, Renderer& gfx) override {
        for (int i = 0; i < 500; i++) {
            int x = rand() % SCREEN_WIDTH + 1;
            int y = rand() % SCREEN_HEIGHT + 1;
            if (back.pixel(x, y) < 100)
                back.pixel(x, y, 255);
        }
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                int ortho = back.pixel(x + 1, y) + back.pixel(x + 2, y + 1) + back.pixel(x + 1, y + 2) + back.pixel(x, y + 1);
                int diagonal = back.pixel(x, y) + back.pixel(x + 2, y) + back.pixel(x + 2, y + 2) + back.pixel(x, y + 2);
                // int color = (int)(((float)ortho + 0.7f * (float)diagonal) / 8.0f);
                int color = (ortho + diagonal) / 8;
                gfx.pixel(x, y, color);
            }
        }
        back.blit(gfx, 1, 1);
    }

    void onRender(App& app, Renderer& gfx) override{};

   private:
    Texture back;
};

int main() {
    EffectScene scene;
    App& app = App::getInstance();
    app.init("Pixel Effect", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
    app.setScene(&scene);
    app.run();
}