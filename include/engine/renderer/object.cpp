#include "Object.hpp"

Object::Object(const std::shared_ptr<Mesh>& mesh)
    : mesh(mesh), position(0.0f), rotation(0.0f), scale(1.0f) {}

void Object::update() {
    for (const auto& component : components) {
        component->update();
    }
}

void Object::draw() const {
    mesh->draw();
}

void Object::addComponent(const std::shared_ptr<Component>& component) {
    components.push_back(component);
}
