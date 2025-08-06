#include <algorithm>

#ifndef SPARSE_ARRAY_TPP
    #define SPARSE_ARRAY_TPP

#include "sparse_array.hpp"

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//
// CONSTRUCTORS & DESTRUCTORS
//
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

template <typename Component>
sparse_array<Component>::sparse_array() :
    _data()
{}

template <typename Component>
sparse_array<Component>::sparse_array(sparse_array const& other) :
    _data(other._data)
{}

template <typename Component>
sparse_array<Component>::sparse_array(sparse_array&& other) noexcept :
    _data(std::move(other._data))
{}

template <typename Component>
sparse_array<Component>::~sparse_array()
{}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//
// OPERATORS OVERLOAD
//
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

template <typename Component>
sparse_array<Component>& sparse_array<Component>::operator=(sparse_array const& other)
{
    if (this != &other) {
        _data = other._data;
    }
    return *this;
}

template <typename Component>
sparse_array<Component>& sparse_array<Component>::operator=(sparse_array&& other) noexcept
{
    if (this != &other) {
        _data = std::move(other._data);
    }
    return *this;
}

template <typename Component>
typename sparse_array<Component>::reference_type sparse_array<Component>::operator[](size_t idx)
{
    ensure_size(idx);
    return _data.at(idx);
}

template <typename Component>
typename sparse_array<Component>::const_reference_type sparse_array<Component>::operator[](size_t idx) const
{
    return _data.at(idx);
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//
// ITERATORS
//
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


template <typename Component>
typename sparse_array<Component>::iterator sparse_array<Component>::begin()
{
    return _data.begin();
}

template <typename Component>
typename sparse_array<Component>::const_iterator sparse_array<Component>::begin() const
{
    return _data.begin();
}

template <typename Component>
typename sparse_array<Component>::const_iterator sparse_array<Component>::cbegin() const
{
    return _data.cbegin();
}

template <typename Component>
typename sparse_array<Component>::iterator sparse_array<Component>::end()
{
    return _data.end();
}

template <typename Component>
typename sparse_array<Component>::const_iterator sparse_array<Component>::end() const
{
    return _data.end();
}

template <typename Component>
typename sparse_array<Component>::const_iterator sparse_array<Component>::cend() const
{
    return _data.cend();
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//
// SIZE
//
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


template <typename Component>
typename sparse_array<Component>::size_type sparse_array<Component>::size() const
{
    return _data.size();
}

template <typename Component>
void sparse_array<Component>::ensure_size(size_type size)
{
    if (_data.size() <= size) {
        _data.resize(size + 1);
    }
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//
// INSERTION
//
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


template <typename Component>
typename sparse_array<Component>::reference_type sparse_array<Component>::insert_at(size_type pos, Component const& value)
{
    ensure_size(pos);
    _data[pos] = value;
    return _data[pos];
}

template <typename Component>
typename sparse_array<Component>::reference_type sparse_array<Component>::insert_at(size_type pos, Component&& value)
{
    ensure_size(pos);
    _data[pos] = std::move(value);
    return _data[pos];
}

template <typename Component>
template <class... Params>
typename sparse_array<Component>::reference_type sparse_array<Component>::emplace_at(size_type pos, Params&&... params)
{
    ensure_size(pos);
    _data[pos].emplace(std::forward<Params>(params)...);
    return _data[pos];
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//
// ERASE
//
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


template <typename Component>
void sparse_array<Component>::erase(size_type pos)
{
    if (pos < _data.size()) {
        _data[pos].reset();
    }
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//
// GETTER
//
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


template <typename Component>
typename sparse_array<Component>::size_type sparse_array<Component>::get_index(value_type const& opt) const
{
    auto it = std::find(_data.begin(), _data.end(), opt);
    return it != _data.end() ? std::distance(_data.begin(), it) : _data.size();
}

#endif

