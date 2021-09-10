#ifndef INCLUDE_CANARY_LOG_HPP
#define INCLUDE_CANARY_LOG_HPP

#include <spdlog/spdlog.h>

namespace Canary {

extern std::shared_ptr<spdlog::logger> logger;

} /* namespace Canary */

#endif /* INCLUDE_CANARY_LOG_HPP */