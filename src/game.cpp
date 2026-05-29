#include <variant>
#include "game.hpp"
#include "screen.hpp"
#include "utils.hpp"

Game::Game() {
    this->m_MenuHistory.push(std::make_unique<MainScreen>());
}

void Game::run() {
    while (!this->m_MenuHistory.empty()) {
        ScreenAction action = this->m_MenuHistory.top()->Show();

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