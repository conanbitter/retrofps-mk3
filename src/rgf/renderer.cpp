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

    framebuffer = new uint8_t[width * height];
}

Renderer::~Renderer() {
    delete[] framebuffer;

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
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, framebuffer);
}

void Renderer::update_palette() {
    glBindTexture(GL_TEXTURE_2D, pal_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 1, GL_RGB, GL_UNSIGNED_BYTE, palette);
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
    std::fill_n(framebuffer, width * height, color);
}

void Renderer::putPixel(int x, int y, uint8_t color) {
    x %= width;
    if (x < 0) x += width;

    y %= height;
    if (y < 0) y += height;

    framebuffer[x + y * width] = color;
}

void Renderer::putLine(uint8_t colors[], int x, int y, int length) {
    if (length < 0 || length + x + y * width > width * height) {
        length = width * height - x + y * width;
    }
    std::copy(colors, colors + length, framebuffer + x + y * width);
}

void Renderer::fillRect(int x, int y, int rectWidth, int rectHeight, uint8_t color) {
    if (x > width || y > height || rectWidth < 0 || rectHeight < 0) return;
    if (x < 0) {
        rectWidth += x;
        x = 0;
    }
    if (x + rectWidth > width) {
        rectWidth = width - x;
    }
    int y1 = y > 0 ? y : 0;
    int y2 = y + rectHeight < height ? y + rectHeight : height;
    for (int i = y1; i < y2; i++) {
        std::fill_n(framebuffer + x + i * width, rectWidth, color);
    }
}

void Renderer::setColor(uint8_t index, Color color) {
    palette[index] = color;
}

void Renderer::setPalette(Color colors[], uint8_t startIndex, unsigned int count) {
    if (startIndex + count > 256) {
        count = 256 - startIndex;
    }
    std::copy(colors, colors + count, palette + startIndex);
    update_palette();
}