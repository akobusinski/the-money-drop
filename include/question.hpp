#pragma once
#include <array>
#include <string_view>

enum QuestionCategory {
    HISTORIA,
    GEOGRAFIA,
    NAUKA,
    SPORT,
    MUZYKA,
    FILM_I_TV,
    LITERATURA,
    TECHNOLOGIA,
    MATEMATYKA,
    PRZYRODA,
    JEDZENIE_I_KUCHNIA,
    KULTURA_I_SZTUKA,
    CIEKAWOSTKI,
    JEZYKI_I_SLOWA,
    POLSKA,
};

class Question {
    private:
        std::string_view m_Question;
        std::array<std::string_view, 4> m_PossibleAnswers;
        int m_PossibleAnswersSize = 2;
        int m_CorrectAnswerIndex;
    public:
        // holy staircase
        constexpr Question(std::string_view question, std::initializer_list<std::string_view> answers, int correct_index)
            : m_Question(question), m_PossibleAnswersSize(answers.size()), m_CorrectAnswerIndex(correct_index) {
                std::size_t i = 0;
                for (std::string_view v : answers) {
                    this->m_PossibleAnswers[i++] = v;
                }
        }
    
        bool is_correct(size_t index) const {
            return index == this->m_CorrectAnswerIndex;
        }
};
