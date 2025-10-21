#pragma once

#include <vector>
#include <stdint.h>

// to zaenkoduje to gowno iwgl
class Encoder {
    public:
        Encoder();
        ~Encoder();
        void encode(void *buf, size_t len);

    private:
        std::vector<char> output_buffer;
};