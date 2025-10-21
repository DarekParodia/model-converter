#include "encoder.h"

using json = nlohmann::json;

Model::Model() {
}

Model::~Model() {
}

types::model *Model::parseJsonStatic(std::ifstream &file) {
    logger->info("Parsing json");
    if(!file)
        throw std::ios_base::failure("Failed to open file");

    json          json_data = json::parse(file);

    types::model *model     = new types::model;
    model->layer_count      = json_data["layers"].size();
    logger->debug("Layer count: " + std::to_string(model->layer_count));

    model->layers = new types::layer[model->layer_count];
    for(int i = 0; i < model->layer_count; i++) {
        types::layer layer;
        layer.weights.size = json_data["layers"][i]["weights"].size();
        layer.biases.size  = json_data["layers"][i]["biases"].size();
        logger->debug(std::format("Layer[{}] Weigths: {}, Biases: {}", i, layer.weights.size, layer.biases.size));

        // weights
        layer.weights.array = new types::float_array[layer.weights.size];
        for(int j = 0; j < layer.weights.size; j++) {
            types::float_array weight;
            weight.size  = json_data["layers"][i]["weights"][j].size();
            weight.array = new float[weight.size];
            for(int k = 0; k < weight.size; k++)
                weight.array[k] = json_data["layers"][i]["weights"][j][k].get<float>();
            layer.weights.array[j] = weight;
        }

        // biases
        layer.biases.array = new float[layer.biases.size];
        for(int j = 0; j < layer.biases.size; j++)
            layer.biases.array[j] = json_data["layers"][i]["biases"][j].get<float>();
        model->layers[i] = layer;
    }

    return model;
}

std::vector<char> Model::serializeStatic(types::model *model) {
    if(model == nullptr) throw std::runtime_error("model cannot be null");
    std::vector<char> output;
    return output;
}

void Model::parseJson(std::ifstream &file) {
    if(this->model != nullptr) delete this->model;
    this->model = Model::parseJsonStatic(file);
};
std::vector<char> Model::serialize() {
    return Model::serializeStatic(this->model);
};