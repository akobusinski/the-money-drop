#pragma once
#include <cstdint>
#include <optional>
#include <filesystem>
#include <span>
#include <utility>
#include <vector>
#include "question.hpp"

using CategorisedQuestion = std::pair<QuestionCategory, OwnedQuestion>;

constexpr const std::size_t           k_Rounds = 8;
constexpr const std::size_t           k_CategoriesPerRound = 2;
constexpr const int                   k_StartingMoney = 1'000'000;
constexpr const std::string_view      k_SaveFileName = "save.file";
constexpr const std::size_t           k_ThreeQuestionsThreshold = 3;
constexpr const std::size_t           k_TwoAnswerRoundCount = 1;
constexpr const std::size_t           k_FourAnswerRoundCount = k_ThreeQuestionsThreshold;
constexpr const std::size_t           k_ThreeAnswerRoundCount = k_Rounds - k_TwoAnswerRoundCount - k_FourAnswerRoundCount;
extern    const std::filesystem::path g_DataDirectory;
extern    const std::filesystem::path g_SaveFilePath;

std::filesystem::path GetDataDirectory(const std::string &name);

struct GenerationStage {
    std::size_t round_count;
    std::span<const std::pair<QuestionCategory, StaticQuestion>> container;
    std::size_t max_answers;
};

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
        void NextRound();
        [[nodiscard]] const std::vector<CategorisedQuestion>* GetCurrentRoundQuestions() const;

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
