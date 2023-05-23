#include "rgf.hpp"

App& App::getInstance() {
    static App theInstance;
    return theInstance;
}

void App::initSDL(const std::string& title) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              windowWidth,
                              windowHeight,
                              SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);
    keyboardState = SDL_GetKeyboardState(nullptr);
}

void App::init(const std::string title, int width, int height, int scale) {
    if (isInitComplete) return;

    frameWidth = width;
    frameHeight = height;
    windowWidth = frameWidth * scale;
    windowHeight = frameHeight * scale;

    initSDL(title);

    isInitComplete = true;
}

App::~App() {
    if (context) {
        SDL_GL_DeleteContext(context);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void App::run() {
    SDL_Event event;
    bool working = true;
    while (working) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    working = false;
                    break;
            }
        }

        SDL_GL_SwapWindow(window);
        SDL_Delay(5);
    }
}