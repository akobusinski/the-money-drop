#pragma once
#include <utility>

#include "game_state.hpp"
#include "screen_actions.hpp"

/**
 * @brief Base class for all screens
 */
class BaseScreen {
    public:
        virtual ~BaseScreen() = default;
        /**
         * @brief Runs the screen until it finishes execution
         * 
         * @return The action that the screen controller should perform next
         */
        virtual ScreenAction Show() = 0;
};

/**
 * @brief The main game screen, handles all game logic
 */
class GameScreen : public BaseScreen {
    public:
        /**
         * @brief Construct a new Game Screen object with a default, randomly generated `GameState`
         */
        GameScreen() = default;
        
        /**
         * @brief Construct a new game screen from an existing game state
         * 
         * @param state The game state to copy
         */
        explicit GameScreen(GameState state) : m_State(std::move(state)) {}

        ScreenAction Show() override;
    private:
        GameState m_State;
};

/**
 * @brief The main menu screen
 */
class MainScreen : public BaseScreen {
    public:
        ScreenAction Show() override;
};
