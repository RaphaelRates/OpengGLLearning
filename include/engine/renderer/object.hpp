#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Mesh.hpp"
#include "Component.hpp"

class Object {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    std::shared_ptr<Mesh> mesh;
    std::vector<std::shared_ptr<Component>> components;

    Object(const std::shared_ptr<Mesh>& mesh);
    void update();
    void draw() const;

    void addComponent(const std::shared_ptr<Component>& component);
};

#endif
