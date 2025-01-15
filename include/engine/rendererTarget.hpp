#ifndef RENDERTARGET_HPP
#define RENDERTARGET_HPP

#include <glad/glad.h>

class RenderTarget {
public:
    unsigned int framebuffer;
    unsigned int texture;

    RenderTarget() : framebuffer(0), texture(0) {}

    void setup(int width, int height);
    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }
    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};

#endif // RENDERTARGET_HPP