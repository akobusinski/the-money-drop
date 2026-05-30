#pragma once
#include <memory>
#include <variant>

// defined in `screen.hpp`, prevents a circular import
class BaseScreen;

/**
 * @brief Action that pushes a new screen onto the stack
 */
struct PushScreen {
    /**
     * @brief The screen instance to push
     */
    std::unique_ptr<BaseScreen> screen;
};

/**
 * @brief Action that closes the current screen
 */
struct CloseScreen {};

/**
 * @brief Variant containing all actions a screen can return
 */
using ScreenAction = std::variant<PushScreen, CloseScreen>;