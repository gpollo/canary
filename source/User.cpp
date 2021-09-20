#include <filesystem>
#if defined(BUILD_LINUX)
#include <cstdlib>
#elif defined(BUILD_WINDOWS)
#include <codecvt>
#include <locale>

#include <shlobj.h>
#else
#error "You must specify a supported operating system"
#endif

#include "User.hpp"
#include "Log.hpp"

namespace Canary::User {

std::filesystem::path getConfigurationDirectoryOnly() {
#ifdef BUILD_LINUX
    auto* envConfigHome = std::getenv("XDG_CONFIG_HOME");
    if (envConfigHome != nullptr) {
        std::filesystem::path pathConfigHome(envConfigHome);
        if (pathConfigHome != "") {
            return pathConfigHome;
        }
    }

    auto* envHome = std::getenv("HOME");
    if (envHome != nullptr) {
        std::filesystem::path pathHome(envHome);
        if (pathHome != "") {
            return pathHome / ".config";
        }
    }

    logger->error("failed to find user configuration directory, defaulting to /tmp/");
    return "/tmp/";
#endif /* BUILD_LINUX */

#ifdef BUILD_WINDOWS
    LPWSTR widePath = NULL;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &widePath);
    if (SUCCEEDED(result)) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        std::string path = converter.to_bytes(widePath);

        CoTaskMemFree(widePath);

        return path;
    }

    logger->error("failed to find user configuration directory, defaulting to C:/");
    return "C:/";
#endif /* BUILD_WINDOWS */
}

std::filesystem::path getConfigurationDirectory() {
    auto directory = getConfigurationDirectoryOnly();

    if (!std::filesystem::create_directories(directory)) {
        logger->error("failed to create configuration directory '{}'", directory.string());
    }

    return directory;
}

} /* namespace Canary::User */
