#include <print>
#include "screen.hpp"

ScreenAction GameScreen::Show() {
    std::println("we are so gaming rn");
    return CloseScreen{};
}