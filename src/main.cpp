#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <sys/time.h>
#include <sys/resource.h>

#include "logger.h"
#include "encoder.h"

bool          encode   = true; // decode if false
std::string   filepath = "";
Model<int8_t> model;

long          getMemoryUsageKB() {
    struct rusage usage;
    if(getrusage(RUSAGE_SELF, &usage) == 0)
        return usage.ru_maxrss; // maximum resident set size (in kilobytes on many systems)
    return -1;
}

int parseArguments(int argc, char *argv[]) {
    try {
        cxxopts::Options options("converter", "AI model converter tool");

        options.add_options()("i,input", "Set a input file path", cxxopts::value<std::string>())("h,help", "Show help message");

        auto result = options.parse(argc, argv);

        if(result.count("help")) {
            std::cout << options.help() << "\n";
            exit(0);
        }

        if(!result.count("input")) {
            logger->error("Error: input file not specified!");
            return 1;
        }

        filepath = result["input"].as<std::string>();

    } catch(const cxxopts::exceptions::parsing &e) {
        logger->error("Error parsing arguments: " + std::string(e.what()));
        logger->info("Use --help for more information");
        return 1;
    } catch(const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

int readFile() {
    logger->info("Reading file contents from path: " + filepath);
    std::ifstream file(filepath);
    if(!file) {
        logger->error("Failed to open file");
        return 1;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    logger->info("File size: " + std::to_string(size) + " bytes");

    return 0;
}

//
int main(int argc, char *argv[]) {
    int status = 0;

    status     = parseArguments(argc, argv);
    if(status != 0) return status;

    std::chrono::steady_clock::time_point begin     = std::chrono::steady_clock::now();
    long                                  begin_mem = getMemoryUsageKB();

    status                                          = readFile();
    if(status != 0) return status;

    std::chrono::steady_clock::time_point read_file_time = std::chrono::steady_clock::now();
    long                                  read_file_mem  = getMemoryUsageKB();

    std::ifstream                         file(filepath);
    try {
        model.parseJson(file);
    } catch(const std::exception &e) {
        logger->error("Error encoding: " + std::string(e.what()));
    }

    std::chrono::steady_clock::time_point json_parse_time = std::chrono::steady_clock::now();
    long                                  json_parse_mem  = getMemoryUsageKB();

    logger->info("Serializing");
    try {
        auto output = model.getModel()->serialize();
        logger->debug("output size: " + std::to_string(output.size()));
        std::ofstream outFile("output.bin"); // open file for writing
        outFile.write(output.data(), output.size());
        outFile.close();
    } catch(const std::exception &e) {
        logger->error("Error Serializing: " + std::string(e.what()));
    }

    std::chrono::steady_clock::time_point serialization_time = std::chrono::steady_clock::now();
    long                                  serialization_mem  = getMemoryUsageKB();

    logger->info("Deserializing");
    types::model<int8_t> model2;
    try {
        std::ifstream inFile("output.bin");
        model2.deserialize(inFile);
    } catch(const std::exception &e) {
        logger->error("Error Deserializing: " + std::string(e.what()));
    }
    std::chrono::steady_clock::time_point deserialization_time = std::chrono::steady_clock::now();
    long                                  deserialization_mem  = getMemoryUsageKB();

    // serialize again to check
    logger->info("Serializing again to verify");
    try {
        auto output = model2.serialize();
        logger->debug("output size: " + std::to_string(output.size()));
        std::ofstream outFile("output2.bin"); // open file for writing
        outFile.write(output.data(), output.size());
        outFile.close();

    } catch(const std::exception &e) {
        logger->error("Error Serializing: " + std::string(e.what()));
    }


    // calculate times and memory usages
    auto read_file_duration           = std::chrono::duration_cast<std::chrono::milliseconds>(read_file_time - begin).count();
    auto json_parse_duration          = std::chrono::duration_cast<std::chrono::milliseconds>(json_parse_time - read_file_time).count();
    auto serialization_duration       = std::chrono::duration_cast<std::chrono::milliseconds>(serialization_time - json_parse_time).count();
    auto deserialization_duration     = std::chrono::duration_cast<std::chrono::milliseconds>(deserialization_time - serialization_time).count();
    auto total_duration               = std::chrono::duration_cast<std::chrono::milliseconds>(serialization_time - begin).count();
    auto read_file_memory_usage       = read_file_mem - begin_mem;
    auto json_parse_memory_usage      = json_parse_mem - read_file_mem;
    auto serialization_memory_usage   = serialization_mem - json_parse_mem;
    auto deserialization_memory_usage = deserialization_mem - serialization_mem;
    auto total_memory_usage           = deserialization_mem - begin_mem;


    logger->info("Time taken (ms): Read file: " + std::to_string(read_file_duration) + " ms, Memory used: " + std::to_string(read_file_memory_usage) + " KB");
    logger->info("Time taken (ms): JSON parse: " + std::to_string(json_parse_duration) + " ms, Memory used: " + std::to_string(json_parse_memory_usage) + " KB");
    logger->info("Time taken (ms): Serialization: " + std::to_string(serialization_duration) + " ms, Memory used: " + std::to_string(serialization_memory_usage) + " KB");
    logger->info("Time taken (ms): Deserialization: " + std::to_string(deserialization_duration) + " ms, Memory used: " + std::to_string(deserialization_memory_usage) + " KB");
    logger->info("Total time (ms): " + std::to_string(total_duration) + " ms, Total memory used: " + std::to_string(total_memory_usage) + " KB");

    return 0;
}
