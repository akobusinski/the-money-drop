#pragma once
#include <array>
#include <string_view>
#include <string>
#include <utility>
#include <vector>

constexpr const std::size_t k_MaxAnswers = 4;

enum class QuestionCategory : std::uint16_t {
    HISTORIA = 0,
    GEOGRAFIA = 1,
    NAUKA = 2,
    SPORT = 3,
    MUZYKA = 4,
    FILM_I_TV = 5,
    LITERATURA = 6,
    TECHNOLOGIA = 7,
    MATEMATYKA = 8,
    PRZYRODA = 9,
    JEDZENIE_I_KUCHNIA = 10,
    KULTURA_I_SZTUKA = 11,
    CIEKAWOSTKI = 12,
    JEZYKI_I_SLOWA = 13,
    POLSKA = 14,

    MAX
};

// WHY CAN I NOT JUST ADD A MEMBER ::Name() FUNCTION
constexpr std::string_view CategoryName(QuestionCategory category) {
    switch (category) {
        case QuestionCategory::HISTORIA: return "Historia";
        case QuestionCategory::GEOGRAFIA: return "Geografia";
        case QuestionCategory::NAUKA: return "Nauka";
        case QuestionCategory::SPORT: return "Sport";
        case QuestionCategory::MUZYKA: return "Muzyka";
        case QuestionCategory::FILM_I_TV: return "Film i TV";
        case QuestionCategory::LITERATURA: return "Literatura";
        case QuestionCategory::TECHNOLOGIA: return "Technologia";
        case QuestionCategory::MATEMATYKA: return "Matematyka";
        case QuestionCategory::PRZYRODA: return "Przyroda";
        case QuestionCategory::JEDZENIE_I_KUCHNIA: return "Jedzenie i kuchnia";
        case QuestionCategory::KULTURA_I_SZTUKA: return "Kultura i sztuka";
        case QuestionCategory::CIEKAWOSTKI: return "Ciekawostki";
        case QuestionCategory::JEZYKI_I_SLOWA: return "Jezyki i slowa";
        case QuestionCategory::POLSKA: return "Polska";

        default: return "???";
    }
}

// I could instead make `StaticQuestion` a `QuestionView` or something and make `OwnedQuestion` expose a `QuestionView OwnedQuestion::View()` method,
// but I don't think that's necessary, technically could save some memory, but only for initial game load, since after a load it needs to dynamically reconstruct
// the questions, so at this stage it's really overengineering the whole thing.
struct StaticQuestion {
    std::string_view question;
    std::array<std::string_view, k_MaxAnswers> answers;
    std::size_t answer_count;
    std::size_t correct_answer;

    constexpr StaticQuestion(
        const std::string_view question,
        const std::array<std::string_view, k_MaxAnswers> &answers,
        const std::size_t answer_count,
        const std::size_t correct_answer
    ) : question(question), answers(answers), answer_count(answer_count), correct_answer(correct_answer) {}
};

struct OwnedQuestion {
    std::string question;
    std::vector<std::string> answers;
    std::size_t correct_answer;

    OwnedQuestion(
        std::string question,
        std::vector<std::string> answers,
        const std::size_t correct_answer
    ) : question(std::move(question)), answers(std::move(answers)), correct_answer(correct_answer) {}

    explicit OwnedQuestion(StaticQuestion other) : correct_answer(other.correct_answer) {
        this->question = other.question;
        this->answers.reserve(other.answer_count);
        
        for (std::size_t index = 0; index < other.answer_count; index++) {
            this->answers.emplace_back(other.answers[index]);
        }
    }
    
    [[nodiscard]] bool IsCorrect(const size_t index) const {
        return index == this->correct_answer;
    }
};
