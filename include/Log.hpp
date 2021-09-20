#ifndef INCLUDE_CANARY_LOG_HPP
#define INCLUDE_CANARY_LOG_HPP

#include <filesystem>

#include <spdlog/spdlog.h>

template <>
struct fmt::formatter<std::filesystem::path> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const std::filesystem::path& path, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "{}", std::string(path));
    }
};

namespace Canary {

extern std::shared_ptr<spdlog::logger> logger;

} /* namespace Canary */

#endif /* INCLUDE_CANARY_LOG_HPP */