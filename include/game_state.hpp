#pragma once
#include <cstdint>
#include <optional>
#include <filesystem>
#include <span>
#include <utility>
#include <vector>
#include "question.hpp"

using CategorisedQuestion = std::pair<QuestionCategory, OwnedQuestion>;

constexpr const std::size_t           k_Rounds = 8; ///< The amount of rounds in a newly constructed game
constexpr const std::size_t           k_CategoriesPerRound = 2; ///< The amount of categories that the player can choose from each round
constexpr const int                   k_StartingMoney = 1'000'000; ///< The amount of money that the player starts with
constexpr const std::string_view      k_SaveFileName = "save.file"; ///< The name of the save file
constexpr const std::size_t           k_ThreeQuestionsThreshold = 3; ///< When should we drop from four questions to three
constexpr const std::size_t           k_TwoAnswerRoundCount = 1; ///< The amount of rounds with only two possible answers
constexpr const std::size_t           k_FourAnswerRoundCount = k_ThreeQuestionsThreshold; ///< The amount of rounds with four possible answers
constexpr const std::size_t           k_ThreeAnswerRoundCount = k_Rounds - k_TwoAnswerRoundCount - k_FourAnswerRoundCount; ///< The amount of rounds with three possible answers
extern    const std::filesystem::path g_DataDirectory; ///< The directory which stores all game data
extern    const std::filesystem::path g_SaveFilePath; ///< The path to the save file

std::filesystem::path GetDataDirectory(const std::string &name);

/**
 * @brief Helper for generating random questions
 */
struct GenerationStage {
    std::size_t round_count; ///< Amount of rounds that the stage handles
    std::span<const std::pair<QuestionCategory, StaticQuestion>> container; ///< The container with the new question pool
    std::size_t max_answers; ///< Max amount of answers in each round handled by this stage
};

/**
 * @brief Stores the player state
 */
class GameState {
    public:
        /**
         * @brief Constructs a new Game State object with random questions
         */
        GameState();

        /**
         * @brief Construct a new Game State object
         * 
         * @param money Players amount of money
         * @param rounds Categories that the player can choose from
         * @param current_round Which round the player is currently on
         */
        GameState(
            const std::uint32_t money,
            std::vector<std::vector<CategorisedQuestion>> rounds,
            const std::size_t current_round
        ) : m_Money(money), m_Rounds(std::move(rounds)), m_CurrentRound(current_round) {};

        /**
         * @brief Tries to load the save file
         * 
         * @return std::optional<GameState> If `std::nullopt`, save file either doesn't exist or is corrupted, otherwise a constructed Game State object.
         */
        static std::optional<GameState> TryLoad();

        /**
         * @brief Saves the `GameState` to a save file.
         */
        void Save() const;

        /**
         * @brief Increments the current round counter
         */
        void NextRound();
        /**
         * @brief Get the questions for the current round
         * 
         * @return const std::vector<CategorisedQuestion>* 
         */
        [[nodiscard]] const std::vector<CategorisedQuestion>* GetCurrentRoundQuestions() const;

        /**
         * @brief Get what round the player is on
         * 
         * @return std::size_t 
         */
        [[nodiscard]] std::size_t Round() const {
            return this->m_CurrentRound;
        }

        /**
         * @brief Get how much money the player currently has
         * 
         * @return std::uint32_t 
         */
        [[nodiscard]] std::uint32_t Money() const {
            return this->m_Money;
        }

        /**
         * @brief Set the amount of money the player has
         * 
         * @param newMoney New amount of money
         */
        void SetMoney(const std::uint32_t newMoney) {
            this->m_Money = newMoney;
        }
    private:
        std::uint32_t m_Money;
        std::vector<std::vector<CategorisedQuestion>> m_Rounds;
        std::size_t m_CurrentRound;
};
