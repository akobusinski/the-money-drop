#include <filesystem>
#include <random>
#include <array>
#include "game_state.hpp"

std::filesystem::path GetDataDirectory(const std::string &name) {
#if defined(_WIN32)
    const char* appdata = std::getenv("APPDATA");
    if (appData) return std::string(appdata) + "\\" + name;
#elif defined(__linux__)
    const char* xdg = std::getenv("XDG_DATA_HOME");
    if (xdg) return std::string(xdg) + "/" + name;

    const char* home = std::getenv("HOME");
    if (home) return std::string(home) + "/.local/share/" + name;
#elif defined(__APPLE__)
    const char* home = std::getenv("HOME");
    if (home) return std::string(home) + "/Library/Application Support/" + name;
#endif
    return "./" + name;
}

std::filesystem::path g_DataDirectory = GetDataDirectory("postaw-na-milion");

// TODO:
// GameState::GameState() {
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     this->m_Questions = std::array {};
// }


std::optional<GameState> GameState::TryLoad() {
    if (!std::filesystem::is_directory(g_DataDirectory)) return {};
    auto savefile = g_DataDirectory.append(k_SavefileName);
    // TODO: load sum

    return {};
}

void GameState::Save() {
    std::filesystem::create_directories(g_DataDirectory);
    auto savefile = g_DataDirectory.append(k_SavefileName);
    // TODO: save sum
}