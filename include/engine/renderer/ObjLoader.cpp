#include "ObjLoader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

ObjLoader::ObjLoader() {}
ObjLoader::~ObjLoader() {}

void ObjLoader::LoadFromFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw std::runtime_error("Erro ao abrir o arquivo: " + fileName);
    }

    std::vector<Position> vertices;
    std::vector<Normal> normals;
    std::string line;
    char currentMtlName[100] = {0};

    while (std::getline(file, line))
    {
        if (StartWith(line, "mtllib"))
        {
            char mtlFileName[100];
            sscanf_s(line.c_str(), "mtllib %s", mtlFileName, sizeof(mtlFileName));
            LoadMaterialFile(mtlFileName);
        }
        else if (StartWith(line, "v "))
        {
            Position pos;
            sscanf_s(line.c_str(), "v %f %f %f", &pos.x, &pos.y, &pos.z);
            vertices.push_back(pos);
        }
        else if (StartWith(line, "vn "))
        {
            Normal n;
            sscanf_s(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
            normals.push_back(n);
        }
        else if (StartWith(line, "usemtl"))
        {
            sscanf_s(line.c_str(), "usemtl %s", currentMtlName, sizeof(currentMtlName));
        }
        else if (StartWith(line, "f "))
        {
            int v1, v2, v3, n1, n2, n3;
            sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
            AddVertexData(v1, n1, currentMtlName, vertices, normals);
            AddVertexData(v2, n2, currentMtlName, vertices, normals);
            AddVertexData(v3, n3, currentMtlName, vertices, normals);
        }
    }

    file.close();
}

void ObjLoader::AddVertexData(int vId, int nId, const char* mtl, std::vector<Position>& vertices, std::vector<Normal>& normals)
{
    Color c = mMaterialMap[mtl];
    Position p = vertices[vId - 1];
    Normal n = normals[nId - 1];

    mVertexData.push_back(p.x);
    mVertexData.push_back(p.y);
    mVertexData.push_back(p.z);
    mVertexData.push_back(c.r);
    mVertexData.push_back(c.g);
    mVertexData.push_back(c.b);
    mVertexData.push_back(n.x);
    mVertexData.push_back(n.y);
    mVertexData.push_back(n.z);
}

bool ObjLoader::StartWith(std::string& line, const char* text)
{
    return line.rfind(text, 0) == 0;
}

void ObjLoader::LoadMaterialFile(const char* filename)
{
    if (!filename) {
        std::cerr << "Erro: Nome do arquivo é nulo!" << std::endl;
        return;
    }

    std::string path = "./OBJ/" + std::string(filename);
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Falha ao carregar o arquivo de materiais: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string currentMaterial;

    while (std::getline(file, line)) {
        if (StartWith(line, "newmtl")) {
            std::istringstream iss(line.substr(7)); // Ignora "newmtl "
            iss >> currentMaterial;

            if (currentMaterial.empty()) {
                std::cerr << "Material sem nome no arquivo: " << filename << std::endl;
                continue;
            }

            mMaterialMap[currentMaterial] = Color(); // Inicializa o material
        } 
        else if (StartWith(line, "Kd")) {
            if (currentMaterial.empty()) {
                std::cerr << "Cor Kd definida antes de um material válido no arquivo: " << filename << std::endl;
                continue;
            }

            std::istringstream iss(line.substr(3)); // Ignora "Kd "
            float r, g, b;
            if (iss >> r >> g >> b) {
                mMaterialMap[currentMaterial] = { r, g, b }; // Define a cor do material
            } else {
                std::cerr << "Erro ao analisar valores Kd no arquivo: " << filename << " na linha: " << line << std::endl;
            }
        }
    }

    file.close();
}

std::vector<float> ObjLoader::GetVertexData()
{
    return mVertexData;
}

int ObjLoader::GetVertexCount()
{
    return static_cast<int>(mVertexData.size() / 9);
}

void ObjLoader::Draw()
{
    if (mVertexData.empty())
    {
        std::cerr << "Nenhum dado de vértice carregado para desenhar." << std::endl;
        return;
    }

    GLuint vertexArray, vertexBuffer;
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(float), mVertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, GetVertexCount());

    glBindVertexArray(0);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);
}
