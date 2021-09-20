#include <filesystem>
#include <fstream>
#include <optional>

#include "User.hpp"
#include "UserConfiguration.hpp"
#include "Log.hpp"

namespace Canary::UserConfiguration {

static nlohmann::json config;

static inline std::optional<std::filesystem::path> getConfigurationDirectory() {
    auto directory = User::getConfigurationDirectory() / "canary";
    if (!std::filesystem::exists(directory)) {
        if (!std::filesystem::create_directory(directory)) {
            logger->error("failed to create configuration directory '{}'", directory.string());
            return {};
        }

        logger->info("creating configuration directory '{}'", directory.string());
    }

    if (!std::filesystem::is_directory(directory)) {
        logger->error("path '{}' is not a directory", directory.string());
        return {};
    }

    return directory;
}

void load() {
    auto directoryOpt = getConfigurationDirectory();
    if (!directoryOpt.has_value()) {
        logger->error("failed to obtain configuration directory, configuration will not be loaded");
        return;
    }
    auto directory = directoryOpt.value();
    auto file = directory / "config.json";

    if (!std::filesystem::exists(file)) {
        logger->warn("no configuration file found at '{}'", file.string());
        return;
    }

    std::ifstream fileStream(file);
    fileStream >> config;
}

void save() {
    auto directoryOpt = getConfigurationDirectory();
    if (!directoryOpt.has_value()) {
        logger->error("failed to obtain configuration directory, configuration will not be saved");
        return;
    }
    auto directory = directoryOpt.value();
    auto file = directory / "config.json";

    std::ofstream fileStream(file);
    fileStream << config;
}

nlohmann::json getKey(const std::string& key, const nlohmann::json& defaultData) {
    if (config.contains(key)) {
        return config[key];
    }

    setKey(key, defaultData);
    return defaultData;
}

void setKey(const std::string& key, const nlohmann::json& data) {
    config[key] = data;
    save();
}

} /* namespace Canary::UserConfiguration */
