#pragma once

#include <vector>

namespace types {
    typedef struct float_array {
            unsigned int size;
            float       *array;
    } float_array;

    typedef struct float_array_2d {
            unsigned int size;
            float_array *array;
    } float_array_2d;

    typedef struct layer {
            float_array_2d weights;
            float_array    biases;
    } layer;

    typedef struct model {
            unsigned int layer_count;
            layer       *laysers;
    } model;
} // namespace types