#pragma once
#include <memory>
#include <stack>
#include "screen.hpp"

class Game {
    public:
        Game();
        ~Game() = default;

        void Run();
    private:
        std::stack<std::unique_ptr<BaseScreen>> m_MenuHistory;
};