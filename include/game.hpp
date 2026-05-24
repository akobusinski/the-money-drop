#pragma once
#include <memory>
#include <stack>
#include "screen.hpp"

class Game {
    public:
        Game();
        ~Game() = default;

        void run();
    private:
        std::stack<std::unique_ptr<BaseScreen>> m_MenuHistory;
};