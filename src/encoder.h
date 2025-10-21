#pragma once

#include <vector>
#include <stdint.h>
#include <fstream>
#include <format>
#include <exception>
#include <nlohmann/json.hpp>

#include "logger.h"
#include "types.h"

// to zaenkoduje to gowno iwgl
class Encoder {
    public:
        Encoder();
        ~Encoder();
        void encode(std::ifstream &file);

    private:
        types::model   model;
        nlohmann::json json_data;
};