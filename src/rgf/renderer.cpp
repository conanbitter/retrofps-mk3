#include "rgf.hpp"
#include "shaders.hpp"
#include <string>

const float quad[6][4] = {
    {-1.0f, -1.0f, 0.0f, 1.0f},
    {-1.0f, 1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 0.0f},
    {1.0f, -1.0f, 1.0f, 1.0f},
};

const char* vertexShaderCode = R"(
    #version 150

    in vec2 vert;
    in vec2 vertUV;

    out vec2 fragUV;

    void main() {
        gl_Position = vec4(vert.x, vert.y, 0.0, 1.0);
        fragUV = vertUV;
    }
)";

const char* fragmentShaderCode = R"(
    #version 150
    uniform sampler2D tex;
    uniform sampler2D pal;

    in vec2 fragUV;

    out vec4 outputColor;

    const float k = 255.0 / 256.0;

    void main() {
        float c = texture(tex, fragUV).r * k;
        outputColor = texture(pal, vec2(c,0));
    }
)";

void Renderer::init(int width, int height) {
    this->width = width;
    this->height = height;

    // Init shader
    program = compileShaderProgram(vertexShaderCode, fragmentShaderCode);
    glUseProgram(program);
    GLint tex_loc = glGetUniformLocation(program, "tex");
    GLint pal_loc = glGetUniformLocation(program, "pal");
    GLint vert_loc = glGetAttribLocation(program, "vert");
    GLint vertUV_loc = glGetAttribLocation(program, "vertUV");

    // Init buffers
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vert_loc);
    glVertexAttribPointer(vert_loc, 2, GL_FLOAT, false, 4 * 4, nullptr);
    glEnableVertexAttribArray(vertUV_loc);
    glVertexAttribPointer(vertUV_loc, 2, GL_FLOAT, false, 4 * 4, (const void*)(2 * 4));
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * 4, &quad, GL_STATIC_DRAW);

    // Init textures
    glGenTextures(1, &frame_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glGenTextures(1, &pal_texture);
    glBindTexture(GL_TEXTURE_2D, pal_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glUniform1i(tex_loc, 0);
    glUniform1i(pal_loc, 1);

    framebuffer.resize(width * height);
}

Renderer::~Renderer() {
    if (glIsTexture(frame_texture)) {
        glDeleteTextures(1, &frame_texture);
    }
    if (glIsTexture(pal_texture)) {
        glDeleteTextures(1, &frame_texture);
    }
    if (glIsBuffer(vbo)) {
        glDeleteBuffers(1, &vbo);
    }
    if (glIsVertexArray(vao)) {
        glDeleteVertexArrays(1, &vao);
    }
    if (glIsProgram(program)) {
        glDeleteProgram(program);
    }
}

void Renderer::update() {
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, framebuffer.data());
}

void Renderer::update_palette() {
    glBindTexture(GL_TEXTURE_2D, pal_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 1, GL_RGB, GL_UNSIGNED_BYTE, palette.data());
}

void Renderer::present() {
    glUseProgram(program);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pal_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::clear(uint8_t color) {
    std::fill(framebuffer.begin(), framebuffer.end(), color);
}

void Renderer::putPixel(int x, int y, uint8_t color) {
    if (
        x < 0 ||
        x >= width ||
        y < 0 ||
        y >= height) return;
    framebuffer[x + y * width] = color;
}

void Renderer::setColor(uint8_t index, Color color) {
    if (index >= 0 && index < 256) {
        palette[index] = color;
    }
}

void Renderer::setPalette(const Palette& colors, uint8_t startIndex, int count) {
    if (count < 0) {
        count = colors.size();
    }
    if (startIndex + count > 256) {
        count = 256 - startIndex;
        if (count <= 0) return;
    }
    std::copy(colors.begin(), colors.begin() + count, palette.begin() + startIndex);
    update_palette();
}

void Renderer::setPalette(const TexturePack& tpak) {
    std::copy(tpak.palette.begin(), tpak.palette.end(), palette.begin() + tpak.paletteOffset);
    update_palette();
}

static void boundsCheck(Rect& rect, int x, int y, int width, int height, int texWidth, int texHeight) {
    if (rect.x < 0) {
        rect.w -= rect.x;
        rect.x = 0;
    }
    if (x < 0) {
        rect.w -= x;
        x = 0;
    }
    if (rect.y < 0) {
        rect.h -= rect.y;
        rect.y = 0;
    }
    if (y < 0) {
        rect.h -= y;
        y = 0;
    }
    if (rect.x + rect.w >= texWidth) {
        rect.w = texWidth - rect.x;
    }
    if (x + rect.w >= width) {
        rect.w = width - x;
    }
    if (rect.y + rect.h >= texHeight) {
        rect.h = texHeight - rect.y;
    }
    if (y + rect.h >= height) {
        rect.h = height - y;
    }
}

void Renderer::blit(const Texture& tex, int x, int y, Rect rect) {
    boundsCheck(rect, x, y, width, height, tex.width, tex.height);
    if (rect.w <= 0 || rect.h <= 0) return;

    for (int ty = 0; ty < rect.h; ty++) {
        std::copy(
            tex.data.begin() + rect.x + (rect.y + ty) * tex.width,
            tex.data.begin() + rect.x + (rect.y + ty) * tex.width + rect.w,
            framebuffer.begin() + x + (y + ty) * width);
    }
}

void Renderer::blit(const Texture& tex, int x, int y) {
    blit(tex, x, y, Rect(0, 0, tex.width, tex.height));
}

void Renderer::blitTransp(const Texture& tex, int x, int y, Rect rect) {
    boundsCheck(rect, x, y, width, height, tex.width, tex.height);
    if (rect.w <= 0 || rect.h <= 0) return;

    uint8_t tranpColor = tex.transparent_color;
    for (int ty = 0; ty < rect.h; ty++) {
        for (int tx = 0; tx < rect.w; tx++) {
            uint8_t color = tex.data[rect.x + tx + (rect.y + ty) * tex.width];
            if (color == tranpColor) continue;
            framebuffer[x + tx + (y + ty) * width] = color;
        }
    }
}

void Renderer::blitTransp(const Texture& tex, int x, int y) {
    blitTransp(tex, x, y, Rect(0, 0, tex.width, tex.height));
}