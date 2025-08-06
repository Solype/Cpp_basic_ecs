#include "entity.hpp"

ecs::entity::entity(size_t const &s) :
    _id(s)
{}

ecs::entity::~entity()
{}

ecs::entity::operator size_t() const
{
    return _id;
}

bool ecs::entity::operator==(entity const &other) const
{
    return _id == other._id;
}

bool ecs::entity::operator!=(entity const &other) const
{
    return _id != other._id;
}
