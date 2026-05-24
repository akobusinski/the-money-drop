#include <print>
#include "screen.hpp"

ScreenAction MainScreen::Update() {
    std::println("hi");
    return PushScreen { std::make_unique<GameScreen>() };
}

ScreenAction GameScreen::Update() {
    std::println("we are so gaming rn");
    return CloseScreen{};
}