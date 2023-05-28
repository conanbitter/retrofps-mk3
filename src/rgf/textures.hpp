#pragma once

#include <vector>
#include <string>
#include <map>
#include "renderer.hpp"

class Texture {
   public:
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    uint8_t getPixel(int x, int y);

   private:
    int width;
    int height;
    int transparent_color;
    std::vector<uint8_t> data;

    friend class TexturePack;
    friend Renderer;
};

class TexturePack {
   public:
    void load(const std::string filename);
    int count() const { return textures.size(); }
    Texture& getTexture(const std::string name);
    Texture& getTexture(int id);
    int getTextureId(const std::string name);
    const Palette& getPalette() const { return palette; }
    int getPaletteOffset() const { return paletteOffset; }

   private:
    std::vector<Texture> textures;
    std::map<std::string, int> names;
    Palette palette;
    int paletteOffset;

    friend Renderer;
};