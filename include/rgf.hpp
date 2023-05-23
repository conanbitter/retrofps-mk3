#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>

class App {
   public:
    static App& getInstance();
    void init(const std::string title, int width, int height, int scale = 1);
    void run();

   private:
    bool isInitComplete;
    int frameWidth;
    int frameHeight;
    int windowWidth;
    int windowHeight;
    float aspectRatio;
    SDL_Window* window;
    SDL_GLContext context;
    const Uint8* keyboardState;

    void initSDL(const std::string& title);
    App() : isInitComplete{false} {};
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    ~App();
};