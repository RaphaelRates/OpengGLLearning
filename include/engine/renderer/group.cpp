#include "Group.hpp"

void Group::addObject(const std::shared_ptr<Object>& object) {
    objects.push_back(object);
}

void Group::update() {
    for (const auto& object : objects) {
        object->update();
    }
}

void Group::draw() const {
    for (const auto& object : objects) {
        object->draw();
    }
}
