#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <engine/renderer/Mesh.hpp>
#include <engine/texture/shader.hpp>

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
public:
    std::vector<Mesh> meshes;

    Model(const std::string &path)
    {
        loadModel(path);
    }

    void draw() const
    {
        for (const auto &mesh : meshes)
            mesh.draw();
    }

private:
    void loadModel(const std::string &path)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        loadOBJ(path, vertices, indices);

        meshes.emplace_back(vertices, indices);
    }

    void loadOBJ(const std::string &path, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
    {
        std::vector<glm::vec3> tempPositions;
        std::vector<glm::vec2> tempTexCoords;
        std::vector<glm::vec3> tempNormals;

        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Erro ao abrir o arquivo: " << path << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream lineStream(line);
            std::string prefix;
            lineStream >> prefix;

            if (prefix == "v")
            { // Posições dos vértices
                glm::vec3 position;
                lineStream >> position.x >> position.y >> position.z;
                tempPositions.push_back(position);
            }
            else if (prefix == "vt")
            { // Coordenadas de textura
                glm::vec2 texCoord;
                lineStream >> texCoord.x >> texCoord.y;
                tempTexCoords.push_back(texCoord);
            }
            else if (prefix == "vn")
            { // Normais
                glm::vec3 normal;
                lineStream >> normal.x >> normal.y >> normal.z;
                tempNormals.push_back(normal);
            }
            else if (prefix == "f")
            { // Faces
                unsigned int posIndex[3], texIndex[3], normIndex[3];
                char slash; // Para lidar com "v/t/n" no formato OBJ
                for (int i = 0; i < 3; ++i)
                {
                    lineStream >> posIndex[i] >> slash >> texIndex[i] >> slash >> normIndex[i];
                    --posIndex[i]; // Ajustar índice para 0-based
                    --texIndex[i];
                    --normIndex[i];

                    Vertex vertex = {};
                    vertex.Position = tempPositions[posIndex[i]];
                    vertex.TexCoords = tempTexCoords[texIndex[i]];
                    vertex.Normal = tempNormals[normIndex[i]];

                    vertices.push_back(vertex);
                    indices.push_back(static_cast<unsigned int>(vertices.size()) - 1);
                }
            }
        }
        file.close();
    }
};

#endif