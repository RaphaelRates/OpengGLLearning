#ifndef COMPONENT_H
#define COMPONENT_H

class Component {
public:
    virtual void update() = 0;
    virtual ~Component() {}
};

#endif
