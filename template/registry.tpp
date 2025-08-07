#include <stdexcept>
#include <iostream>
#include <chrono>

#ifndef REGISTRY_TPP_
    #define REGISTRY_TPP_

#include "registry.hpp"

namespace ecs {


/////////////////////////////////////////////////////////////
//
// Handle the components in the registry
//
/////////////////////////////////////////////////////////////
template <class Component>
sparse_array<Component> &registry::register_component()
{
    std::type_index type = typeid(Component);

    if (this->_components_arrays.find(type) == this->_components_arrays.end()) {
        this->_components_arrays[type] = sparse_array<Component>();
        this->_destructors[type] = [&](registry &reg, entity const &e) {
            auto &array = reg.get_components<Component>();
            if (array.size() <= e) {
                return;
            }
            if (auto& optional_component = array[e]; optional_component.has_value()) {
                optional_component.reset();
            }
        };

    }
    return std::any_cast<sparse_array<Component>&>(this->_components_arrays[type]);
}

template <class Component>
void registry::unregister_component()
{
    std::type_index type = typeid(Component);

    this->_destructors.erase(type);
    this->_components_arrays.erase(type);
}

template <class Component>
sparse_array<Component>& registry::get_components()
{
    std::type_index type = typeid(Component);

    if (this->_components_arrays.find(type) == this->_components_arrays.end()) {
        std::string error("Component not registered in registry ");
        throw std::runtime_error(error + type.name());
    }
    std::any_cast<sparse_array<Component>&>(this->_components_arrays.at(type));
    return std::any_cast<sparse_array<Component>&>(this->_components_arrays.at(type));
}

template <class Component>
sparse_array<Component> const& registry::get_components() const
{
    std::type_index type = typeid(Component);
    if (this->_components_arrays.find(type) == this->_components_arrays.end()) {
        std::string error("Component not registered in registry ");
        throw std::runtime_error(error + type.name());
    }
    return std::any_cast<sparse_array<Component> const&>(this->_components_arrays.at(type));
}


/////////////////////////////////////////////////////////////
//
// handle components of an entity
//
/////////////////////////////////////////////////////////////
template<typename Component, typename ...Params >
typename sparse_array<Component>::reference_type registry::emplace_component(entity const &to, Params &&...p)
{
    auto &components = this->get_components<Component>();
    return components.emplace_at(to, std::forward<Params>(p)...);
}

template<typename Component>
void registry::remove_component(entity const &from)
{
    auto &type = typeid(Component);
    auto &destructor = this->_destructors.at(type);
    destructor(*this, from);
}


/////////////////////////////////////////////////////////////
//
// handle the different systems
//
/////////////////////////////////////////////////////////////

template <class... Components, typename Function>
void registry::register_system(Function&& f)
{
    auto &id = typeid(Function);

    _systems.emplace(id, 
        [f = std::forward<Function>(f), this](registry& reg, int elapsed_time) mutable {
            static auto copy = f;
            copy(reg, elapsed_time, this->get_components<Components>()...);
        }
    );
}


template <typename Function>
void registry::run_single_system()
{
    auto &id = typeid(Function);
    auto it = this->_systems.find(id);

    if (it != this->_systems.end()) {
        it->second(*this, 0);
    }
}

template<typename Function>
void registry::unregister_syste()
{
    auto id = typeid(Function);

    this->_enabled_systems.erase(id);
    this->_systems.erase(id);
}

template<typename Function>
void registry::enable_system()
{
    auto &id = typeid(Function);
    auto it = this->_systems.find(id);

    if (it != this->_systems.end()) {
        this->_enabled_systems.insert(id);
    }
}

template<typename Function>
void registry::disable_system()
{
    auto &id = typeid(Function);

    this->_enabled_systems.erase(id);
}

}

#endif /* !REGISTRY_TPP_ */
