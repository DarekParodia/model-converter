#include <cxxopts.hpp>
#include <iostream>
#include <string>

#include "logger.h"

std::string filepath;

int         main(int argc, char *argv[]) {
    // argument handling
    try {
        cxxopts::Options options("converter", "AI model converter tool");

        options.add_options()("i,input", "Set a input file path", cxxopts::value<std::string>())("h,help", "Show help message");

        auto result = options.parse(argc, argv);

        if(result.count("help")) {
            std::cout << options.help() << "\n";
            return 0;
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
