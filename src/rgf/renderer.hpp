#pragma once

#include "glad/gl.h"
#include <vector>
#include <array>

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
    void fillRect(int x, int y, int width, int height, uint8_t color);
    void setColor(uint8_t index, Color color);
    void setPalette(const std::vector<Color>& colors, uint8_t startIndex = 0, int count = -1);

    friend App;

   private:
    int width;
    int height;
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint frame_texture;
    GLuint pal_texture;
    std::vector<uint8_t> framebuffer;
    std::array<Color, 256> palette;

    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    ~Renderer();
    void init(int width, int height);
    void update();
    void update_palette();
    void present();
};
