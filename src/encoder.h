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
        static types::model *parseJsonStatic(std::ifstream &file);

        void                 parseJson(std::ifstream &file);

        types::model        *getModel();

    private:
        types::model *model = nullptr;
};