#ifndef ZIPPER_TPP_
    #define ZIPPER_TPP_

#include "zipper_iterator.hpp"
#include "zipper.hpp"
#include <iostream>
#include <algorithm> // Pour std::min
#include <utility>   // Pour std::size et std::end

// Constructeur principal
template <class... Containers>
zipper<Containers...>::zipper(Containers&... cs)
    : _begin(std::make_tuple(std::begin(cs)...)),
      _end(_compute_end(cs...)),
      _size(_compute_size(cs...)) {}

// Méthode begin
template <class... Containers>
typename zipper<Containers...>::iterator zipper<Containers...>::begin()
{
    return iterator(_begin, _size);
}

// Méthode end
template <class... Containers>
typename zipper<Containers...>::iterator zipper<Containers...>::end()
{
    auto it = iterator(_end, _size);
    it._idx = _size;
    return it;
}

// Calcul de la taille minimale parmi les conteneurs
template <class... Containers>
size_t zipper<Containers...>::_compute_size(Containers&... containers)
{
    return std::min({std::size(containers)...}); // Trouve la taille minimale des conteneurs
}

// Construction d'un iterator_tuple pour l'itérateur de fin
template <class... Containers>
typename zipper<Containers...>::iterator_tuple zipper<Containers...>::_compute_end(Containers&... containers)
{
    auto ends = std::make_tuple(std::end(containers)...);
    return ends;
}

#endif /* !ZIPPER_TPP_ */
