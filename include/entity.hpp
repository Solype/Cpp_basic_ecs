#include <cstddef>

#ifndef ENTITY_HPP_
    #define ENTITY_HPP_


namespace ecs {


/**
 * @class entity
 * @brief Entity class, used to get information about an entity using its id and the ecs::registry
 */

class entity {
    public:

        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        //      Entity
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        /// @name Entity
        /// @{
            

        /**
         * @brief Construct a new entity object
         * @param s the id of the entity
         */
        explicit entity(size_t const &s);

        /**
         * @brief default destructor
         */
        ~entity();

        /// @}
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        //      OPERATORS
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////
        /// @name Operators
        /// @{

        /**
         * @brief cast the entity to a size_t
         * @return the id of the entity as a size_t
        */
        operator size_t() const;

        /**
         * @brief cast the entity to a size_t
         * @return the id of the entity as a size_t
        */
        bool operator==(entity const &other) const;

        /**
         * @brief cast the entity to a size_t
         * @return the id of the entity as a size_t
        */
        bool operator!=(entity const &other) const;

        /// @}

    private:
        size_t _id;
};


}

#endif /* !ENTITY_HPP_ */
