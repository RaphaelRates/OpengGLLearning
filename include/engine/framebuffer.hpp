#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Framebuffer {
public:
    Framebuffer(unsigned int width, unsigned int height)
        : width(width), height(height), framebuffer(0), textureColorbuffer(0), rbo(0) {
        init();
    }

    ~Framebuffer() {
        // Cleanup resources
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteTextures(1, &textureColorbuffer);
        glDeleteRenderbuffers(1, &rbo);
    }

    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }

    void unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool isComplete() const {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    unsigned int getColorAttachment() const {
        return textureColorbuffer;
    }

private:
    unsigned int width, height;
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int rbo;

    void init() {
        // Generate and bind the framebuffer
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        // Create a texture for the color attachment
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

        // Create a renderbuffer object for depth and stencil attachment
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        // Check if framebuffer is complete
        if (!isComplete()) {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }

        // Unbind the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};

