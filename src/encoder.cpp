#include "encoder.h"

using json = nlohmann::json;

Encoder::Encoder() {
}

Encoder::~Encoder() {
}

void Encoder::encode(std::ifstream &file) {
    logger->info("Parsing json");
    if(!file)
        throw std::ios_base::failure("Failed to open file");

    this->json_data = json::parse(file);

    types::model model;

    model.layer_count = this->json_data["layers"].size();
    logger->debug("Layer count: " + std::to_string(model.layer_count));

    for(int i = 0; i < model.layer_count; i++) {
        types::layer layer;
        layer.weights.size = this->json_data["layers"][i]["weights"].size();
        layer.biases.size  = this->json_data["layers"][i]["biases"].size();
        logger->debug(std::format("Layer[{}] Weigths: {}, Biases: {}", i, layer.weights.size, layer.biases.size));

        // weights
        layer.weights.array = new types::float_array[layer.weights.size];
        for(int j = 0; j < layer.weights.size; j++) {
            types::float_array weight;
            weight.size  = this->json_data["layers"][i]["weights"][j].size();
            weight.array = new float[weight.size];
            for(int k = 0; k < weight.size; k++)
                weight.array[k] = this->json_data["layers"][i]["weights"][j][k];
            layer.weights.array[j] = weight;
        }

        // biases
        layer.biases.array = new float[layer.biases.size];
        for(int j = 0; j < layer.biases.size; j++)
            layer.biases.array[j] = this->json_data["layers"][i]["biases"][j];
        model.laysers[i] = layer;
    }
}