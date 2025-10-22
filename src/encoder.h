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
template <typename T>
class Model {
    public:
        Model();
        ~Model();

        static types::model<T> *parseJsonStatic(std::ifstream &file);

        void                    parseJson(std::ifstream &file);

        types::model<T>        *getModel();

    private:
        types::model<T> *model = nullptr;
        static T         getValueFromJson(const nlohmann::json &j);
};