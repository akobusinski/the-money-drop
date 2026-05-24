#include <variant>
#include "game.hpp"
#include "screen.hpp"

// https://en.cppreference.com/cpp/utility/variant/visit2#Example
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

Game::Game() {
    this->m_MenuHistory.push(std::make_unique<MainScreen>());
}

void Game::run() {
    while (!this->m_MenuHistory.empty()) {
        ScreenAction action = this->m_MenuHistory.top()->Update();

        std::visit(overloaded {
            [&](CloseScreen) {
                this->m_MenuHistory.pop();
            },
            [&](PushScreen &arg) {
                this->m_MenuHistory.push(std::move(arg.menu));
            },
        }, action);
    }
}