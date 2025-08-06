#include "sparse_array.hpp"
#include "entity.hpp"

#ifndef ECS_SYSTEM_HPP_
    #define ECS_SYSTEM_HPP_

    #include "zipper.hpp"

namespace ecs {

/**
 * @brief registry class
 * This class is used to store the components of the entities
 */
class registry;

/**
 * @brief Base class for all the systems. All systems must be classes that inherit from this class
 * @tparam Components the components used by the system to process
 * @code
 * class MySystem : public ecs::system<MyComponent1, MyComponent2> { // inherit from ecs::system and use template
 *     void operator()(
 *              ecs::sparse_array<MyComponent1>& sa1,
 *              ecs::sparse_array<MyComponent2>& sa2) const
 *      {
 *         // do something
 *      }
 * };
 * @endcode
 */
template <class... Components>
class isystem {
    public:
        /**
         * @brief Default constructor
         */
        isystem() = default;

        /**
         * @brief Default destructor
         */
        virtual ~isystem() = default;

        /**
         * @brief Call the system
         * @tparam Components the components used by the system
         * @note This function is pure virtual and is mandatory to implement because it is the entry point of the system
         */
        virtual void operator()(registry &, int, sparse_array<Components> &...) = 0;
};

}

#endif /* !SYSTEM_HPP_ */
