#ifndef ZIPPER_ITERATOR_HPP_
    #define ZIPPER_ITERATOR_HPP_

#include <tuple>
#include <iterator>
#include <optional>
#include <type_traits>
#include <utility>

template <class... Containers>
class zipper;

/**
 * @brief A zipper iterator
 * This class is used to iterate over multiple containers at the same time
 * @tparam Containers the containers to iterate over
 */
template <class... Containers>
class zipper_iterator {
    public:
        template <class Container>
        using iterator_t = decltype(std::begin(std::declval<Container&>()));

        template <class Container>
        using it_reference_t = typename std::iterator_traits<iterator_t<Container>>::reference;

        using value_type = std::tuple<size_t, it_reference_t<Containers>...>;
        using reference = value_type;
        using pointer = void;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using iterator_tuple = std::tuple<iterator_t<Containers>...>;

        zipper_iterator(iterator_tuple const& it_tuple, size_t max);
        zipper_iterator(zipper_iterator const& z) = default;

        /**
         * @brief Increment the iterator
         */
        zipper_iterator& operator++();

        /**
         * @brief Increment the iterator
         * @param int
         */
        zipper_iterator operator++(int);

        /**
         * @brief multiply the iterator
         */
        value_type operator*();

        /**
         * @brief Compare two iterators
         * @param lhs the left iterator
         * @param rhs the right iterator
         * @return true if the iterators are equals
         * @return false otherwise
         */
        friend bool operator==(zipper_iterator const& lhs, zipper_iterator const& rhs) {
            return lhs._idx == rhs._idx;
        }

        /**
         * @brief Compare two iterators
         * @param lhs the left iterator
         * @param rhs the right iterator
         * @return true if the iterators are different
         * @return false otherwise
         */
        friend bool operator!=(zipper_iterator const& lhs, zipper_iterator const& rhs) {
            return !(lhs == rhs);
        }

        friend class zipper<Containers...>;

    private:
        template <size_t... Is>

        /**
         * @brief Increment all the iterators
         * @tparam Is the index of the containers
         */
        void incr_all(std::index_sequence<Is...>);

        /**
         * @brief Convert the iterators to a value
         * @tparam Is the index of the containers
         */
        template <size_t... Is>
        value_type to_value(std::index_sequence<Is...>);

        iterator_tuple _current;
        size_t _max;
        size_t _idx;
        static constexpr auto _seq = std::index_sequence_for<Containers...>{};
};

#include "zipper_iterator.tpp"

#endif /* !ZIPPER_ITERATOR_HPP_ */
