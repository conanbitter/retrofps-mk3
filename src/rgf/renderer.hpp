#pragma once

#include "glad/gl.h"

class App;

#pragma pack(push, 1)
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
#pragma pack(pop)

class Renderer {
   public:
    void clear(uint8_t color);
    void putPixel(int x, int y, uint8_t color);
    void putLine(uint8_t colors[], int x = 0, int y = 0, int length = -1);
    void fillRect(int x, int y, int width, int height, uint8_t color);
    void setColor(uint8_t index, Color color);
    void setPalette(Color colors[], uint8_t startIndex = 0, unsigned int count = 256);

    friend App;

   private:
    int width;
    int height;
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint frame_texture;
    GLuint pal_texture;
    uint8_t* framebuffer = nullptr;
    Color palette[256];

    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    ~Renderer();
    void init(int width, int height);
    void update();
    void update_palette();
    void present();
};
