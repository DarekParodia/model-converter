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

        static AI::model_data_t<T> *parseJsonStatic(std::ifstream &file);

        void                        parseJson(std::ifstream &file);

        AI::model_data_t<T>        *getModel();

    private:
        AI::model_data_t<T> *model = nullptr;
        static T             getValueFromJson(const nlohmann::json &j);
};