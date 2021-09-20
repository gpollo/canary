#ifndef INCLUDE_CANARY_USER_CONFIGURATION_HPP
#define INCLUDE_CANARY_USER_CONFIGURATION_HPP

#include <filesystem>

#include <nlohmann/json.hpp>

namespace Canary::UserConfiguration {

void load();

void save();

nlohmann::json getKey(const std::string& key, const nlohmann::json& defaultData);

void setKey(const std::string& key, const nlohmann::json& data);

} /* namespace Canary::UserConfiguration */

#endif /* INCLUDE_CANARY_USER_CONFIGURATION_HPP */