#pragma once

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

inline auto logger = spdlog::stdout_color_mt("console");

/**
 * @brief Sets logger log level etc.
 * calling this function isn't required for using logger
 */
inline void init_logger() {
#if defined(PRODUCTION) // set log level
    logger->set_level(spdlog::level::off);
#else
    logger->set_level(spdlog::level::trace);
#endif
}