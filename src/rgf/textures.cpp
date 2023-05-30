#include "textures.hpp"
#include <fstream>

void Texture::wrapCoords(int &x, int &y) const {
    x %= width;
    if (x < 0) x += width;
    y %= height;
    if (y < 0) y += height;
}

uint8_t Texture::pixel(int x, int y) const {
    wrapCoords(x, y);
    return data[x + y * width];
}

void Texture::pixel(int x, int y, uint8_t color) {
    wrapCoords(x, y);
    data[x + y * width] = color;
}

int readU8(std::ifstream &file) {
    uint8_t data;
    file.read((char *)&data, 1);
    return data;
}

int readU32(std::ifstream &file) {
    uint32_t data;
    file.read((char *)&data, 4);
    return data;
}

int readI16(std::ifstream &file) {
    int16_t data;
    file.read((char *)&data, 2);
    return data;
}

void TexturePack::load(const std::string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    // Palette
    size_t paletteSize = readU8(file);
    palette.resize(paletteSize);
    paletteOffset = readU8(file);
    file.read((char *)palette.data(), sizeof(Color) * paletteSize);

    // Textures
    size_t textureCount = readU32(file);
    textures.reserve(textureCount);
    for (int i = 0; i < textureCount; i++) {
        Texture &newTex = textures.emplace_back();

        char name[16];
        file.read(name, 16);
        names[name] = i;

        newTex.width = readU32(file);
        newTex.height = readU32(file);
        newTex.transparent_color = readI16(file);
        newTex.data.resize(newTex.width * newTex.height);
        file.read((char *)newTex.data.data(), newTex.width * newTex.height);
    }
}

Texture &TexturePack::getTexture(std::string name) {
    auto index = names.find(name);
    return index == names.end() ? textures[0] : textures[index->second];
}

Texture &TexturePack::getTexture(int id) {
    if (id > textures.size()) {
        return textures[0];
    }
    return textures[id];
}

int TexturePack::getTextureId(std::string name) {
    auto index = names.find(name);
    return index == names.end() ? -1 : index->second;
}