#pragma once
#include <memory>
#include <stack>
#include "screen.hpp"

/**
 * @brief Object that manages the currently rendered screen
 */
class Game {
    public:
        /**
         * @brief Creates a new Game
         */
        Game();
        ~Game() = default;

        /**
         * @brief Runs the main render loop
         */
        void Run();
    private:
        std::stack<std::unique_ptr<BaseScreen>> m_MenuHistory;
};