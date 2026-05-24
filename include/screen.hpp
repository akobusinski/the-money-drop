#pragma once
#include "screen_actions.hpp"

class BaseScreen {
    public:
        virtual ~BaseScreen() = default;
        virtual ScreenAction Show() = 0;
};


class GameScreen : public BaseScreen {
    public:
        ScreenAction Show() override;
};

class MainScreen : public BaseScreen {
    public:
        ScreenAction Show() override;
};