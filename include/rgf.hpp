#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>

#pragma pack(push, 1)
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
#pragma pack(pop)

class App;

#include "../src/rgf/renderer.hpp"

class Scene {
   public:
    virtual void onLoad(App& app, Renderer& gfx) { isLoaded = true; }
    virtual void onUnload(App& app, Renderer& gfx) { isLoaded = false; }
    virtual void onUpdate(float deltaTime, App& app, Renderer& gfx) {}
    virtual void onRender(App& app, Renderer& gfx) {}

   protected:
    bool isLoaded;
};

class App {
   public:
    static App& getInstance();
    void init(const std::string title, int width, int height, int scale = 1);
    void run();
    void requestExit();
    void setScene(Scene* newScene);

   private:
    bool isInitComplete;
    bool isRunning;
    int frameWidth;
    int frameHeight;
    int windowWidth;
    int windowHeight;
    float aspectRatio;
    SDL_Window* window;
    SDL_GLContext context;
    const Uint8* keyboardState;
    Scene dummyScene;
    Scene* currentScene;
    Renderer renderer;

    void initSDL(const std::string& title);
    App() : isInitComplete{false}, dummyScene(), currentScene{&dummyScene} {};
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    ~App();
};