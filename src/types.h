#pragma once

#include <vector>
#include <type_traits>
#include <cstddef>
#include <memory>

namespace AI {
    // helper functions
    // Append a single POD value to a vector<char>
    template <typename T>
    inline void append(std::vector<char> &vec, const T &value) {
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

    enum class ModelType : uint8_t {
        FLOAT32   = 1,
        FLOAT64   = 2,
        INT8      = 3,
        INT16     = 4,
        INT32     = 5,
        UNDEFINED = 255
    };

    template <typename U>
    static constexpr ModelType getTypeTag() {
        return ModelType::UNDEFINED;
    };

    template <typename T>
    struct array_1d {
            unsigned int      size; // it's count, not bytes
            T                *array;

            std::vector<char> serialize() const {
                std::vector<char> output;
                append(output, size);
                if(size > 0 && array != nullptr)
                    append(output, array, static_cast<size_t>(size) * sizeof(T));
                return output;
            }

            void deserialize(std::ifstream &file) {
                file.read(reinterpret_cast<char *>(&size), sizeof(size));
                if(size > 0) {
                    array = new T[size];
                    file.read(reinterpret_cast<char *>(array), static_cast<size_t>(size) * sizeof(T));
                }
            }

            void free() {
                if(array) delete array;
            }
    };

    template <typename T>
    struct array_2d {
            unsigned int      size;
            array_1d<T>      *array;

            std::vector<char> serialize() const {
                std::vector<char> output;
                append(output, size);
                for(size_t i = 0; i < static_cast<size_t>(size); ++i)
                    if(array != nullptr)
                        append(output, array[i].serialize());

                return output;
            }

            void deserialize(std::ifstream &file) {
                file.read((char *) &size, sizeof(size));
                if(size > 0) {
                    array = new array_1d<T>[size];
                    for(size_t i = 0; i < static_cast<size_t>(size); ++i)
                        array[i].deserialize(file);
                }
            }

            void free() {
                if(size > 0)
                    for(size_t i = 0; i < static_cast<size_t>(size); ++i)
                        array[i].free();
                if(array) delete array;
            }
    };

    template <typename T>
    struct layer_t {
            array_2d<T>       weights;
            array_1d<T>       biases;

            std::vector<char> serialize() const {
                std::vector<char> output;
                append(output, weights.serialize());
                append(output, biases.serialize());
                return output;
            }

            void deserialize(std::ifstream &file) {
                weights.deserialize(file);
                biases.deserialize(file);
            }

            void free() {
                weights.free();
                biases.free();
            }
    };

    template <typename T>
    struct model_data_t {
            unsigned int      layer_count;
            layer_t<T>       *layers;

            std::vector<char> serialize() const {
                std::vector<char> output;

                // save the data type tag
                ModelType typeTag = getTypeTag<T>();
                append(output, typeTag);

                append(output, layer_count);
                for(size_t i = 0; i < static_cast<size_t>(layer_count); ++i)
                    if(layers != nullptr)
                        append(output, layers[i].serialize());

                return output;
            }

            void deserialize(std::ifstream &file) {
                // check if file is readable
                if(!file)
                    throw std::ios_base::failure("Failed to open file");

                file.seekg(1, std::ios::cur);                                            // skip type tag
                file.read(reinterpret_cast<char *>(&layer_count), sizeof(unsigned int)); // read layer_count
                if(layer_count > 0) {
                    layers = new layer_t<T>[layer_count]; // create layers
                    for(size_t i = 0; i < static_cast<size_t>(layer_count); ++i)
                        layers[i].deserialize(file);
                }
            }

            void free() {
                if(layer_count > 0)
                    for(size_t i = 0; i < static_cast<size_t>(layer_count); ++i)
                        layers[i].free();

                if(layers) delete layers;
            }
    };

    template <>
    constexpr ModelType getTypeTag<float>() {
        return ModelType::FLOAT32;
    }

    template <>
    constexpr ModelType getTypeTag<double>() {
        return ModelType::FLOAT64;
    }

    template <>
    constexpr ModelType getTypeTag<int8_t>() {
        return ModelType::INT8;
    }

    template <>
    constexpr ModelType getTypeTag<int16_t>() {
        return ModelType::INT16;
    }

    template <>
    constexpr ModelType getTypeTag<int32_t>() {
        return ModelType::INT32;
    }

    inline ModelType getModelDataType(std::ifstream &file) {
        // check if file is readable
        if(!file)
            throw std::ios_base::failure("Failed to open file");

        // read type
        ModelType type;

        file.read((char *) &type, 1);
        return type;
    }

} // namespace AI