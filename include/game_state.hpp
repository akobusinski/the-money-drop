#pragma once
#include <cstdint>
#include <optional>
#include <filesystem>
#include <array>
#include "question.hpp"

using CategorisedQuestion = std::pair<QuestionCategory, Question>;

constexpr const size_t           k_QuestionsCount = 8;
constexpr const int              k_StartingMoney = 1'000'000;
constexpr const std::string_view k_SavefileName = "save.file";
extern std::filesystem::path g_DataDirectory;

std::filesystem::path GetDataDirectory(const std::string &name);

class GameState {
    public:
        // TODO: GameState();
        GameState(uint32_t money, std::array<std::pair<CategorisedQuestion, CategorisedQuestion>, k_QuestionsCount> questions, std::size_t current_question)
            : m_Money(money), m_Questions(questions), m_CurrentQuestion(current_question) {};
        static std::optional<GameState> TryLoad();
        ~GameState() = default;

        void Save();
    private:
        uint32_t m_Money;
        std::array<std::pair<CategorisedQuestion, CategorisedQuestion>, k_QuestionsCount> m_Questions;
        int m_CurrentQuestion;
};