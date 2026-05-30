#pragma once
#include <array>
#include <string_view>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Maximum amount of answers that stored
 */
constexpr const std::size_t k_MaxAnswers = 4;

/**
 * @brief Categories that questions can belong to
 */
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
/**
 * @brief Converts a `QuestionCategory` into a string
 * 
 * @param category The category to convert
 * @return constexpr std::string_view The name
 */
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

/**
 * @brief A question that can be created at compile time
 * @warning Does not own the data, except for the wrong_answers!
 */
struct StaticQuestion {
    std::string_view question; ///< The question text
    std::string_view correct_answer; ///< The correct answer
    std::array<std::string_view, k_MaxAnswers - 1> wrong_answers; ///< Array containing the incorrect answers 
    std::size_t answer_count; ///< Total number of answers available, including the correct one

    /**
     * @brief Constructs a new question
     * 
     * @param question The question text
     * @param correct_answer The correct answer
     * @param wrong_answers Array containing the incorrect answers
     * @param answer_count Total number of answers available, including the correct one
     */
    constexpr StaticQuestion(
        const std::string_view question,
        const std::string_view correct_answer,
        const std::array<std::string_view, k_MaxAnswers - 1> &wrong_answers,
        const std::size_t answer_count
    ) : question(question), correct_answer(correct_answer), wrong_answers(wrong_answers), answer_count(answer_count) {}
};

/**
 * @brief A question that owns its data
 */
struct OwnedQuestion {
    std::string question; ///< The question text
    std::vector<std::string> answers; ///< Array storing both correct and incorrect answers
    std::size_t correct_answer; ///< Index of the correct answer

    /**
     * @brief Construct a new Owned Question object
     * 
     * @param question The question text
     * @param answers Array storing both correct and incorrect answers
     * @param correct_answer Index of the correct answer
     */
    OwnedQuestion(
        std::string question,
        std::vector<std::string> answers,
        const std::size_t correct_answer
    ) : question(std::move(question)), answers(std::move(answers)), correct_answer(correct_answer) {}
};
