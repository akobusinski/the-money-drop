#pragma once
#include <memory>
#include <variant>

// defined in `screen.hpp`, prevents a circular import
class BaseScreen;

struct PushScreen {
    std::unique_ptr<BaseScreen> menu;
};

struct CloseScreen {};

using ScreenAction = std::variant<PushScreen, CloseScreen>;