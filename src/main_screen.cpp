#include <print>
#include "screen.hpp"

ScreenAction MainScreen::Update() {
    std::println("hi");
    return PushScreen { std::make_unique<GameScreen>() };
}