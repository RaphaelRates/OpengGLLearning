#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <engine/texture/texture.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void draw() const;

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif
