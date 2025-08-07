#include "sparse_array.hpp"
#include "entity.hpp"
#include "isystem.hpp"
#include <unordered_map>
#include <any>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>
#include <set>
#include <functional>
#include <list>
#include <vector>

#ifndef REGISTRY_HPP_
    #define REGISTRY_HPP_

namespace ecs {

/**
 * @brief Diagramme UML pour la classe registry
 *
 * @startuml
 * class registry {
 *   + register_component()
 *   + get_components()
 *   + create_entity()
 *   + entity_from_index()
 *   + delete_entity()
 *   + emplace_component()
 *   + remove_component()
 *   + register_system()
 *   + run_systems()
 * }
 * class sparse_array {
 *   + register_component()
 *   + get_components()
 * }
 * class entity {
 *   + id: int
 *   + getInfo()
 * }
 * registry "1" -- "0..*" sparse_array : contains
 * registry "1" -- "0..*" entity : creates
 * registry "1" -- "0..*" unordered_map : stores
 * registry "1" -- "0..*" std::function : manages
 * @enduml
 */


/**
 * @brief Diagramme de la classe registry
 *
 * @dot
 * digraph registry {
 *     node [shape=record, fontname="Helvetica"];
 *
 *     registry [label="{ registry | + register_component() | + get_components() | + create_entity() | + entity_from_index() | + delete_entity() | + emplace_component() | + remove_component() | + register_system() | + run_systems() }"];
 *     sparse_array [label="{ sparse_array\<\> | + operator[]() }"];
 *     entity [label="{ entity | + id: int | + operator size_t() }"];
 *     std_function [label="{ std::function | + operator()() }"];
 *     unordered_map [label="{ unordered_map | + insert() | + erase() }"];
 *
 *     registry -> sparse_array [label="contains"];
 *     registry -> entity [label="creates"];
 *     registry -> unordered_map [label="stores"];
 *     registry -> std_function [label="manages"];
 * }
 * @enddot
 */

/**
 * @class registry
 * @brief Management of the entities, components and systems
 */
class registry {
    public :

        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        //      Handle the components in the registry
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        /// @name Registering components
        /// @{

        /**
         * @brief Register a component in the registry.
         * @tparam Component 
         * @return The sparse array of the component just registered
         */
        template <class Component>
        sparse_array<Component> &register_component();

        /**
         * @brief Retrieve the sparse array of a registered component
         * @tparam Component 
         * @return the sparse array of the component
         */
        template <class Component>
        sparse_array<Component> &get_components();

        /**
         * @brief Retrieve the sparse array of a registered component but const
         * @tparam Component 
         * @return the sparse array of the component as const
         */
        template <class Component>
        sparse_array<Component> const &get_components() const;

        /**
         * @brief Register a component in the registry.
         * @tparam the component to unregister
         */
        template <class Component>
        void unregister_component();

        /// @}
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        //      Handle the entities in the registry
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        /// @name Handling entities
        /// @{

        /**
         * @brief Give a new entity to the registry and return it
         * @note this function will increment the total entity count
         * or take an entity from the unused entities set
         * @return the new entity
         */
        entity create_entity();

        /**
         * @brief Retrieve an entity from its index
         * @param idx the index of the entity
         * @return the entity
         */
        entity entity_from_index(std::size_t idx);

        /**
         * @brief Delete all the components of an entity
         * @param e the entity to delete and add the unused entity to
         * the unused entities set
         */
        void delete_entity(entity const &e);

        /// @}
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        //      Handle the components in the registry
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        /// @name Handling components
        /// @{

        /**
         * @brief add a component to an entity and return it
         * @tparam Component : the component to add
         * @tparam ...Params : the parameters to pass to the constructor of the component
         * @param to : the entity to add the component to
         * @param ...p : the parameters to pass to the constructor of the component
         * @return return the component just added
         */
        template<typename Component, typename ...Params>
        typename sparse_array<Component>::reference_type emplace_component(entity const &to, Params &&...p);

        /**
         * @brief remove a component from an entity
         * @tparam Component the component to remove
         * @param from the entity to remove the component from
         */
        template<typename Component>
        void remove_component(entity const &from);

        /// @}
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        //      Handle the systems
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        /// @name Handling systems
        /// @{

        /**
         * @brief Add a system to the registry that operates on specific components.
         * This version is used for systems that process entities with specific components.
         * @tparam Components The components used by the system.
         * @tparam Function The type of the system (should inherit from ecs::isystem<Components...>).
         *        The system must implement `void operator()(registry &, Components& ...) const`.
         * @param f The system to add. It is a callable that takes a reference to the registry and
         *          the required components as parameters.
         * @note Requires the specified components to be present in the sparse arrays of the registry.
         *       This function will encapsulate the system call to process entities with matching components.
         */
        template<class... Components, typename Function>
        void register_system(Function&& f);

        // template<class... Components, typename Function>
        // void register_system(Function& f);


        /**
         * @tparam Function, the type of the system
         * it removes the system from the registry.
         */
        template<typename Function>
        void unregister_syste();

        /**
         * @tparam Funciton, the system to enable
         * @brief enable the system, so it runs with with the method "runs_system()".
         * The system needs to be added before being enabled
         */
        template<typename Function>
        void enable_system();

        /**
         * @tparam Function, the system to disable
         * @brief disable the system given. It will no longer runs with run_systems.
         */
        template<typename Function>
        void disable_system();

        /**
         * @brief run all the enabled systems in the registry
         */
        void run_systems();

        /**
         * @brief run a single system in the registry, the system must be 
         * @param system the system to run
         */
        template <typename Function>
        void run_single_system();


        /// @}

    private :

        std::unordered_map<std::type_index, std::any> _components_arrays;

        /**
         * @brief Handle destruction of components
         */
        using destructor_t = std::function<void(registry &, entity const &)>;
        std::unordered_map<std::type_index, destructor_t> _destructors;

        /**
         * @brief Keep track of unused entities and size of the registry
         */
        std::set<entity> _unused_entities;
        std::size_t _total_entity_count = 0;

        /**
         * @brief Handle the systems
         */
        std::unordered_map<std::type_index, std::function<void(registry &, int)>> _systems;
        std::unordered_set<std::type_index> _enabled_systems;

        /**
         * @brief time handler
         */
        int _last_time = 0;
};


}

#include "registry.tpp"

#endif /* !REGISTRY_HPP_ */
