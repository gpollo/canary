#include <spdlog/sinks/stdout_color_sinks.h>

#include "Log.hpp"

namespace Canary {

std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("canary", spdlog::color_mode::automatic);

} /* namespace Canary */