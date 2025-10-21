#pragma once

#include <vector>
#include <type_traits>
#include <cstddef>

namespace types {

    // Append a single POD value to a vector<char>
    template <typename T>
    void append(std::vector<char> &vec, const T &value) {
        static_assert(std::is_trivial<T>::value, "T must be a POD type");
        const char *ptr = reinterpret_cast<const char *>(&value);
        vec.insert(vec.end(), ptr, ptr + sizeof(T));
    }

    // Append another vector<char> to a vector<char>
    inline void append(std::vector<char> &vec, const std::vector<char> &other) {
        vec.insert(vec.end(), other.begin(), other.end());
    }

    // Append a raw buffer to a std::vector<char>
    inline void append(std::vector<char> &vec, const void *buffer, size_t size) {
        const char *ptr = reinterpret_cast<const char *>(buffer);
        vec.insert(vec.end(), ptr, ptr + size);
    }

    typedef struct float_array {
            unsigned int      size; // its count not size
            float            *array;

            std::vector<char> serialize() {
                std::vector<char> output;
                append(output, size);
                append(output, array, size * sizeof(float));
                return output;
            }
    } float_array;

    typedef struct float_array_2d {
            unsigned int      size;
            float_array      *array;

            std::vector<char> serialize() {
                std::vector<char> output;
                append(output, size);
                for(int i = 0; i < size; i++)
                    append(output, array[i].serialize());

                return output;
            }
    } float_array_2d;

    typedef struct layer {
            float_array_2d    weights;
            float_array       biases;

            std::vector<char> serialize() {
                std::vector<char> output;
                append(output, weights.serialize());
                append(output, biases.serialize());
                return output;
            }
    } layer;

    typedef struct model {
            unsigned int      layer_count;
            layer            *layers;

            std::vector<char> serialize() {
                std::vector<char> output;
                append(output, layer_count);
                for(int i = 0; i < layer_count; i++)
                    append(output, layers[i].serialize());

                return output;
            }
    } model;
} // namespace types