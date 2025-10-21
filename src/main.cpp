#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "logger.h"
#include "encoder.h"

bool              encode   = true; // decode if false
std::string       filepath = "";
std::vector<char> file_conents;
Encoder           encoder;


int               parseArguments(int argc, char *argv[]) {
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

    // file_conents.clear();
    // file_conents.resize(size);
    // if(file.read(file_conents.data(), size))

    return 0;
}

//
int main(int argc, char *argv[]) {
    int status = 0;

    status     = parseArguments(argc, argv);
    if(status != 0) return status;

    status = readFile();
    if(status != 0) return status;

    std::ifstream file(filepath);
    try {
        encoder.encode(file);
    } catch(const std::exception &e) {
        logger->error("Error encoding: " + std::string(e.what()));
    }

    return 0;
}
