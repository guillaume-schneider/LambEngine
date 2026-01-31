#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <model.hpp>
#include <transform.hpp>

class Entity
{
public:
    Entity() : id(entityCount++) {}
    ~Entity() {}

    int getId() const { return id; }

    Transform transform;
    // Model model;
private:
    int id;
    static int entityCount;
};

#endif
