#include "registry.hpp"
#include "entity.hpp"

ecs::entity ecs::registry::create_entity()
{
    if (this->_unused_entities.empty()) {
        auto e = entity(this->_total_entity_count);
        this->_total_entity_count++;
        return e;
    }

    // Retirer l'entité de _unused_entities et la retourner directement
    std::size_t entity = *this->_unused_entities.begin();
    this->_unused_entities.erase(this->_unused_entities.begin());
    return this->entity_from_index(entity);
}

ecs::entity ecs::registry::entity_from_index(std::size_t idx)
{
    return entity(idx);
}

void ecs::registry::delete_entity(entity const &e)
{
    for (auto &destructor : this->_destructors) {
        destructor.second(*this, e);
    }
    this->_unused_entities.insert(e);
}

void ecs::registry::run_systems(void)
{
    int elapsed_time = 0;
    auto current_time = std::chrono::steady_clock::now();
    int current_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();

    if (_last_time != 0) {
        elapsed_time = current_ms - _last_time;
    }
    _last_time = current_ms;

    for (auto &system_id : this->_enabled_systems) {
        auto system = this->_systems.at(system_id);
        system(*this, elapsed_time);
    }
}   