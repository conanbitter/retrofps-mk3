#include <SDL.h>

void SDL_test() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("test",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          640,
                                          360,
                                          SDL_WINDOW_OPENGL);
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
    }
    SDL_DestroyWindow(window);
}