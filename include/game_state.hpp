#pragma once
#include <cstdint>
#include <optional>
#include <filesystem>
#include <utility>
#include <vector>
#include "question.hpp"

using CategorisedQuestion = std::pair<QuestionCategory, OwnedQuestion>;

constexpr const std::size_t           k_Rounds = 1; // FIXME: 8;
constexpr const std::size_t           k_CategoriesPerRound = 2;
constexpr const int                   k_StartingMoney = 1'000'000;
constexpr const std::string_view      k_SaveFileName = "save.file";
extern    const std::filesystem::path g_DataDirectory;
extern    const std::filesystem::path g_SaveFilePath;

std::filesystem::path GetDataDirectory(const std::string &name);

class GameState {
    public:
        GameState();
        GameState(
            const std::uint32_t money,
            std::vector<std::vector<CategorisedQuestion>> rounds,
            const std::size_t current_round
        ) : m_Money(money), m_Rounds(std::move(rounds)), m_CurrentRound(current_round) {};
        static std::optional<GameState> TryLoad();
        ~GameState() {
            this->Save();
        }

        void Save() const;
        [[nodiscard]] const std::vector<CategorisedQuestion>* GetCurrentRoundQuestions();

        [[nodiscard]] std::size_t Round() const {
            return this->m_CurrentRound;
        }

        [[nodiscard]] std::uint32_t Money() const {
            return this->m_Money;
        }
        void SetMoney(const std::uint32_t newMoney) {
            this->m_Money = newMoney;
        }
    private:
        std::uint32_t m_Money;
        std::vector<std::vector<CategorisedQuestion>> m_Rounds;
        std::size_t m_CurrentRound;
};
