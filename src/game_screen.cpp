#include <print>
#include "screen.hpp"

ScreenAction GameScreen::Update() {
    std::println("we are so gaming rn");
    return CloseScreen{};
}