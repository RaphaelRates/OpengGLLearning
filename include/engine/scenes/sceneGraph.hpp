// Arquivo: SceneGraph.hpp
#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include <vector>
#include <memory>
#include "Object.hpp"

class SceneGraph {
public:
    std::vector<std::shared_ptr<Object>> objects;

    void addObject(const std::shared_ptr<Object>& object) {
        objects.push_back(object);
    }

    void update() {
        for (const auto& object : objects) {
            object->update();
        }
    }

    void draw() {
        for (const auto& object : objects) {
            object->draw();
        }
    }
};

#endif // SCENEGRAPH_HPP