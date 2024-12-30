#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <memory>
#include "Object.hpp"

class Group {
public:
    std::vector<std::shared_ptr<Object>> objects;

    void addObject(const std::shared_ptr<Object>& object);
    void update();
    void draw() const;
};

#endif
