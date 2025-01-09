#pragma once

#include <iostream>
#include <vector>
#include <map>

class ObjLoader
{
private:
    struct Position
    {
        float x, y, z;
    };
    struct Color
    {
        float r, g, b;
    };
    struct Normal
    {
        float x, y, z;
    };

    void LoadMaterialFile(const char* filename);
    bool StartWith(std::string& line, const char* text);
    void AddVertexData(int vId, int nId, const char* mtl, std::vector<Position>& vertices, std::vector<Normal>& normals);

    std::map<std::string, Color> mMaterialMap;
    std::vector<float> mVertexData;

public:
    ObjLoader();
    ~ObjLoader();

    void LoadFromFile(const std::string& fileName);
    std::vector<float> GetVertexData();
    int GetVertexCount();
    void Draw();
};
