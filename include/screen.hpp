#pragma once
#include "screen_actions.hpp"

class BaseScreen {
    public:
        virtual ~BaseScreen() = default;
        virtual ScreenAction Update() = 0;
};


class GameScreen : public BaseScreen {
    public:
        ScreenAction Update() override;
};

class MainScreen : public BaseScreen {
    public:
        ScreenAction Update() override;
};