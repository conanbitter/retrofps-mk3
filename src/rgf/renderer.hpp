#pragma once

#include "glad/gl.h"
#include <vector>
#include <array>

class TexturePack;
class Texture;

#pragma pack(push, 1)
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
#pragma pack(pop)

struct Rect {
    int x;
    int y;
    int w;
    int h;

    Rect(int x, int y, int width, int height) : x{x}, y{y}, w{width}, h{height} {}
};

typedef std::vector<Color> Palette;

class Renderer {
   public:
    void clear(uint8_t color);
    void putPixel(int x, int y, uint8_t color);
    void setColor(uint8_t index, Color color);
    void setPalette(const Palette& colors, uint8_t startIndex = 0, int count = -1);
    void setPalette(const TexturePack& tpak);
    void blit(const Texture& tex, int x = 0, int y = 0);
    void blit(const Texture& tex, int x, int y, Rect rect);
    void blitTransp(const Texture& tex, int x = 0, int y = 0);
    void blitTransp(const Texture& tex, int x, int y, Rect rect);

    friend class App;

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
