#ifndef ZIPPER_ITERATOR_TPP_
    #define ZIPPER_ITERATOR_TPP_

#include "zipper_iterator.hpp"
#include <iostream>

template <class... Containers>
zipper_iterator<Containers...>::zipper_iterator(iterator_tuple const& it_tuple, size_t max)
    : _current(it_tuple), _max(max), _idx(0)
{}

template <class... Containers>
zipper_iterator<Containers...>& zipper_iterator<Containers...>::operator++() {
    incr_all(_seq);
    ++_idx;
    return *this;
}

template <class... Containers>
zipper_iterator<Containers...> zipper_iterator<Containers...>::operator++(int) {
    zipper_iterator temp = *this;
    ++(*this);
    return temp;
}

template <class... Containers>
typename zipper_iterator<Containers...>::value_type zipper_iterator<Containers...>::operator*() {
    return to_value(_seq);
}

template <class... Containers>
template <size_t... Is>
void zipper_iterator<Containers...>::incr_all(std::index_sequence<Is...>) {
    ((++std::get<Is>(_current)), ...);
}

template <class... Containers>
template <size_t... Is>
typename zipper_iterator<Containers...>::value_type
zipper_iterator<Containers...>::to_value(std::index_sequence<Is...>)
{
    return {this->_idx, (*std::get<Is>(_current))...};
}

#endif /* !ZIPPER_ITERATOR_TPP_ */
