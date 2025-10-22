#include "encoder.h"

using json = nlohmann::json;

template <typename T>
Model<T>::Model() {
}

template <typename T>
Model<T>::~Model() {
}

template <typename T>
types::model<T> *Model<T>::parseJsonStatic(std::ifstream &file) {
    logger->info("Parsing json");
    if(!file)
        throw std::ios_base::failure("Failed to open file");

    json             json_data = json::parse(file);

    types::model<T> *model     = new types::model<T>;
    model->layer_count         = json_data["layers"].size();
    logger->debug("Layer count: " + std::to_string(model->layer_count));

    model->layers = new types::layer<T>[model->layer_count];
    for(int i = 0; i < model->layer_count; i++) {
        types::layer<T> layer;
        layer.weights.size = json_data["layers"][i]["weights"].size();
        layer.biases.size  = json_data["layers"][i]["biases"].size();
        logger->debug(std::format("Layer[{}] Weigths: {}, Biases: {}", i, layer.weights.size, layer.biases.size));

        // weights
        layer.weights.array = new types::array_1d<T>[layer.weights.size];
        for(int j = 0; j < layer.weights.size; j++) {
            types::array_1d<T> weight;
            weight.size  = json_data["layers"][i]["weights"][j].size();
            weight.array = new T[weight.size];
            for(int k = 0; k < weight.size; k++)
                weight.array[k] = getValueFromJson(json_data["layers"][i]["weights"][j][k]);
            layer.weights.array[j] = weight;
        }

        // biases
        layer.biases.array = new T[layer.biases.size];
        for(int j = 0; j < layer.biases.size; j++)
            layer.biases.array[j] = getValueFromJson(json_data["layers"][i]["biases"][j]);
        model->layers[i] = layer;
    }

    return model;
}

template <typename T>
void Model<T>::parseJson(std::ifstream &file) {
    if(this->model != nullptr) delete this->model;
    this->model = Model<T>::parseJsonStatic(file);
};

template <typename T>
types::model<T> *Model<T>::getModel() {
    return this->model;
}
template <typename T>
T Model<T>::getValueFromJson(const nlohmann::json &j) {
    return j.get<T>();
}

template <>
char Model<char>::getValueFromJson(const nlohmann::json &j) {
    return j.get<float>() * 100; // scale to int8
}

// Explicit instantiation for float to ensure symbols are emitted
template class Model<float>;
template class Model<double>;
template class Model<int8_t>;
template class Model<int16_t>;
template class Model<int32_t>;
template class Model<char>;