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
class Model {
    public:
        Model();
        ~Model();
        static types::model     *parseJsonStatic(std::ifstream &file);
        static std::vector<char> serializeStatic(types::model *model);

        void                     parseJson(std::ifstream &file);
        std::vector<char>        serialize();

    private:
        types::model *model = nullptr;
};