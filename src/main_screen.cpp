#include <iostream>
#include <vector>
#include <print>
#include <span>
#include "screen.hpp"
#include "game_state.hpp"
#include "menu.hpp"

enum class MenuOption {
    New,
    Resume,
    Exit,
};

std::vector<MenuItem<MenuOption>> create_options(bool save_file_exists) {
    std::vector<MenuItem<MenuOption>> options;
    options.reserve(save_file_exists ? 3 : 2);
    options.emplace_back("Nowa gra", MenuOption::New);

    if (save_file_exists) {
        options.emplace_back("Wznow gre", MenuOption::Resume);
    }

    options.emplace_back("Wyjdz", MenuOption::Exit);
    return options;
}

ScreenAction MainScreen::Show() {
    std::println("Gra Postaw Na Milion");
    std::println();

    bool save_file_exists = GameState::SaveExists();
    auto options = create_options(save_file_exists);
    MenuOption chosen = ChooseFromMenu("Prosze wybrac jedna z opcji:", std::span { options });

    switch (chosen) {
        case MenuOption::New:
            return PushScreen { std::make_unique<GameScreen>() };
        case MenuOption::Resume:
            std::println("not implemented");
            return CloseScreen {};
        case MenuOption::Exit:
            return CloseScreen {};
    }
}