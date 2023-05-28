#include "rgf.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>

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
    int dx;
    int dy;
    int w;
    int h;
    int back;
    int front;
    TexturePack texs;

    void onLoad(App& app, Renderer& gfx) override {
        Scene::onLoad(app, gfx);
        texs.load("../../../../assets/test.txs");
        back = texs.getTextureId("poster 2");
        front = texs.getTextureId("avatar1");
        gfx.setPalette(pal);
        gfx.setPalette(texs);
        gfx.clear(0);
        w = texs.getTexture(front).getWidth();
        h = texs.getTexture(front).getHeight();

        x = rand() % (SCREEN_WIDTH - w);
        y = rand() % (SCREEN_HEIGHT - h);
        dx = rand() % 2 == 0 ? 1 : -1;
        dy = rand() % 2 == 0 ? 1 : -1;
    }

    void onUnload(App& app, Renderer& gfx) override {
        Scene::onUnload(app, gfx);
    }

    void onUpdate(float deltaTime, App& app, Renderer& gfx) override {
        x += dx;
        y += dy;
        if (x + w >= SCREEN_WIDTH) dx = -1;
        if (y + h >= SCREEN_HEIGHT) dy = -1;
        if (x <= 0) dx = 1;
        if (y <= 0) dy = 1;
    }

    void onRender(App& app, Renderer& gfx) override {
        gfx.blit(texs.getTexture(back));
        gfx.blitTransp(texs.getTexture(front), x, y);
    };
};

int main() {
    std::cout << std::filesystem::current_path() << std::endl;
    TestScene scene;
    App& app = App::getInstance();
    app.init("RetroFPS", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
    app.setScene(&scene);
    app.run();
}