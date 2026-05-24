#include <print>
#include "screen.hpp"

ScreenAction MainScreen::Show() {
    std::println("hi");
    return PushScreen { std::make_unique<GameScreen>() };
}