#include <iostream>
#include <print>
#include "screen.hpp"

ScreenAction MainScreen::Show() {
    std::println("Gra Postaw Na Milion");
    std::println("Prosze wybrac jedna z opcji:");
    std::println("1. Nowa gra");
    std::println("2. Wyjscie");
    std::print("> ");
    int chosen;
    std::cin >> chosen;
    if (!std::cin) {
        std::println("Nieprawidlowa opcja");
        return CloseScreen {};
    }

    if (chosen == 1) {
        return PushScreen { std::make_unique<GameScreen>() };
    } else if (chosen == 2) {
        return CloseScreen {};
    }

    std::println("Nieprawdlowa opcja");
    return CloseScreen {};
}