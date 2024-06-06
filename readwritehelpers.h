// Copyright (C) 2017 Christian Sailer
// Copyright (C) 2018 Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "genlib/exceptions.h"

#include <iostream>
#include <map>
#include <vector>

namespace dXreadwrite {
    // The read/write methods can only be used for vectors of stack allocated types (basic data,
    // POD) read in vector data and write to an existing vector (overwriting its previous contents)
    template <typename T> size_t readIntoVector(std::istream &stream, std::vector<T> &vec) {
        vec.clear();
        unsigned int size;
        stream.read(reinterpret_cast<char *>(&size), sizeof(size));
        if (size > 0) {
            vec.resize(size);
            stream.read(reinterpret_cast<char *>(vec.data()), sizeof(T) * size);
        }
        return size;
    }
    // read in a vector into a new vector
    template <typename T> std::vector<T> readVector(std::istream &stream) {
        std::vector<T> vec;
        readIntoVector(stream, vec);
        return vec;
    }

    template <typename T> void writeVector(std::ostream &stream, const std::vector<T> &vec) {
        // READ / WRITE USES 32-bit LENGTHS (number of elements) for compatibility reasons

        if (vec.size() > size_t(static_cast<unsigned int>(-1))) {
            throw new depthmapX::RuntimeException("Vector exceeded max size for streaming");
        }
        const unsigned int length = static_cast<const unsigned int>(vec.size());
        stream.write(reinterpret_cast<const char *>(&length), sizeof(length));
        if (length > 0) {
            stream.write(reinterpret_cast<const char *>(vec.data()), sizeof(T) * length);
        }
    }

    // read in map data and write to an existing map (overwriting its previous contents)
    template <typename K, typename V>
    size_t readIntoMap(std::istream &stream, std::map<K, V> &map) {
        map.clear();
        unsigned int size;
        stream.read(reinterpret_cast<char *>(&size), sizeof(size));
        for (size_t i = 0; i < size; ++i) {
            K key;
            V value;
            stream.read(reinterpret_cast<char *>(&key), sizeof(K));
            stream.read(reinterpret_cast<char *>(&value), sizeof(V));
            map.insert(std::make_pair(key, value));
        }
        return size;
    }
    // read in a map into a new map
    template <typename K, typename V> std::map<K, V> readMap(std::istream &stream) {
        std::map<K, V> map;
        readIntoMap(stream, map);
        return map;
    }

    template <typename K, typename V>
    void writeMap(std::ostream &stream, const std::map<K, V> &map) {
        // READ / WRITE USES 32-bit LENGTHS (number of elements) for compatibility reasons

        if (map.size() > size_t(static_cast<unsigned int>(-1))) {
            throw new depthmapX::RuntimeException("Map exceeded max size for streaming");
        }
        const unsigned int length = static_cast<const unsigned int>(map.size());
        stream.write(reinterpret_cast<const char *>(&length), sizeof(length));
        for (auto &pair : map) {
            stream.write(reinterpret_cast<const char *>(&pair.first), sizeof(K));
            stream.write(reinterpret_cast<const char *>(&pair.second), sizeof(V));
        }
    }

} // namespace dXreadwrite
