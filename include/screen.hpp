#pragma once
#include "game_state.hpp"
#include "screen_actions.hpp"

class BaseScreen {
    public:
        virtual ~BaseScreen() = default;
        virtual ScreenAction Show() = 0;
};


class GameScreen : public BaseScreen {
    public:
        GameScreen() = default;
        explicit GameScreen(const GameState& state) : m_State(state) {}

        ScreenAction Show() override;
    private:
        GameState m_State;
};

class MainScreen : public BaseScreen {
    public:
        ScreenAction Show() override;
};
