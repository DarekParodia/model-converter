#pragma once

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

// Create logger inline, but configure it immediately
inline auto logger = []() {
    auto log = spdlog::stdout_color_mt("console");

#if defined(PRODUCTION)
    log->set_level(spdlog::level::off);
#else
    log->set_level(spdlog::level::trace); // logger level
#endif

    // Also ensure sinks allow all levels
    for(auto &sink : log->sinks())
        sink->set_level(spdlog::level::trace);

    return log;
}();
