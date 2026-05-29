#include <filesystem>
#include <random>
#include <vector>
#include <print>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>
#include "game_state.hpp"
#include "questions.hpp"

std::filesystem::path GetDataDirectory(const std::string &name) {
#if defined(_WIN32)
    const char* appdata = std::getenv("APPDATA");
    if (appData) return std::string(appdata) + "\\" + name;
#elif defined(__linux__)
    const char* xdg = std::getenv("XDG_DATA_HOME");
    if (xdg) return std::string(xdg) + "/" + name;

    const char* home = std::getenv("HOME");
    if (home) return std::string(home) + "/.local/share/" + name;
#elif defined(__APPLE__)
    const char* home = std::getenv("HOME");
    if (home) return std::string(home) + "/Library/Application Support/" + name;
#endif
    return "./" + name;
}

const std::filesystem::path g_DataDirectory = GetDataDirectory("postaw-na-milion");
const std::filesystem::path g_SaveFilePath  = g_DataDirectory / k_SaveFileName; // TODO: check if this isnt undefined behaviour lol

GameState::GameState() : m_Money(k_StartingMoney), m_CurrentRound(0) {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    constexpr std::size_t questions_count = (k_FourAnswerRoundCount + k_ThreeAnswerRoundCount + k_TwoAnswerRoundCount) * k_CategoriesPerRound;
    // this array makes two assumptions,
    // `max_answers` is descending with every next item (array[i].max_answers > array[i + 1].max_answers)
    // `max_answers` is NEVER more than the amount of answers in ANY question in the `container`
    constexpr auto question_generation_stages = std::array {
        GenerationStage {
            .round_count = k_FourAnswerRoundCount,
            .container = QUESTIONS_FOUR_ANSWERS,
            .max_answers = 4,
        },
        GenerationStage {
            .round_count = k_ThreeAnswerRoundCount,
            .container = QUESTIONS_THREE_ANSWERS,
            .max_answers = 3,
        },
        GenerationStage {
            .round_count = k_TwoAnswerRoundCount,
            .container = QUESTIONS_TWO_ANSWERS,
            .max_answers = 2,
        },
    };

    this->m_Rounds.reserve(questions_count);
    
    std::vector<std::pair<QuestionCategory, StaticQuestion>> question_pool;
    std::size_t required_pool_space = 0;

    for (const auto &[round_count, container, max_answers] : question_generation_stages) {
        required_pool_space += container.size();
        question_pool.reserve(required_pool_space);
        question_pool.append_range(container);

        std::ranges::shuffle(question_pool, rng);

        for (std::size_t round = 0; round < round_count; round++) {
            std::vector<CategorisedQuestion> categories;
            categories.reserve(k_CategoriesPerRound);

            for (std::size_t i = 0; i < k_CategoriesPerRound; i++) {
                const auto &[category, question] = question_pool.back();
                question_pool.pop_back();

                std::vector<std::string> answers;
                answers.reserve(max_answers);
                answers.emplace_back(question.correct_answer);

                std::vector<std::string> wrong_answers;
                wrong_answers.reserve(question.answer_count - 1); // takes only non empty answers out of the question.wrong_answers array
                for (std::size_t index = 0; index < question.answer_count - 1; index++) {
                    wrong_answers.emplace_back(question.wrong_answers[index]); // additionally copy them
                }

                std::ranges::sample( // take out random, incorrect answers
                    wrong_answers,
                    std::back_inserter(answers),
                    max_answers - 1, // remove one, since we already added the correct answer
                    rng
                );

                const std::string correct_answer = answers.front(); // correct answer is always first
                std::ranges::shuffle(answers, rng); // shuffle them again, so the correct answer isn't always first
                const std::size_t correct_answer_index = std::distance( // find the correct answer again
                    answers.begin(),
                    std::ranges::find(answers, correct_answer)
                );

                categories.emplace_back(category, OwnedQuestion {
                    std::string(question.question),
                    answers,
                    correct_answer_index,
                });
            }

            this->m_Rounds.push_back(categories);
        }

        required_pool_space -= round_count * k_CategoriesPerRound;
    }
}

// The save file looks something like this:
// <money> <current_round> <max_rounds> <questions_per_round>
// <answer_count> <category> <correct_answer>
// <question>
// { <answer> x {answer_count} }

std::optional<GameState> GameState::TryLoad() { // I really do wonder if a binary format would be easier than all this string manipulation
    if (!std::filesystem::is_directory(g_DataDirectory)) return {};
    if (!std::filesystem::exists(g_SaveFilePath)) return {};
    
    std::fstream file{g_SaveFilePath, std::fstream::binary | std::fstream::in};
    if (!file) return {};

    std::string line;

    std::uint32_t money;
    std::size_t current_round;
    std::size_t round_count;
    std::size_t questions_per_round;

    { // "header" reading
        if (!std::getline(file, line)) return {};

        std::stringstream stream(line);
        stream >> money >> current_round >> round_count >> questions_per_round;
    }
    
    std::vector<std::vector<CategorisedQuestion>> rounds;
    rounds.reserve(round_count);

    // array of rounds
    for (std::size_t round = 0; round < round_count; round++) {
        std::vector<CategorisedQuestion> questions;
        questions.reserve(questions_per_round);

        // array of questions
        for (std::size_t question = 0; question < questions_per_round; question++) {
            if (!std::getline(file, line)) return {};
            std::stringstream stream(line);

            std::size_t answer_count;
            std::uint16_t category_value;
            std::size_t correct_answer;
            if (!(stream >> answer_count >> category_value >> correct_answer)) return {};

            // the cast is super duper ugly i wish there was a way to add methods to an enum without a namespace 💔💔
            if (category_value > static_cast<std::uint16_t>(QuestionCategory::MAX)) return {};

            std::string question_string;
            if (!std::getline(file, question_string)) return {};

            std::vector<std::string> answers;
            answers.reserve(answer_count);
            
            // array of answers
            for (std::size_t answer = 0; answer < answer_count; answer++) {
                if (!std::getline(file, line)) return {};

                answers.emplace_back(line);
            }

            questions.emplace_back(
                static_cast<QuestionCategory>(category_value), // another nasty cast 💔
                OwnedQuestion {
                    question_string,
                    answers,
                    correct_answer
                }
            );
        }

        rounds.emplace_back(questions);
    }

    file.close();
    return GameState {
        money,
        rounds,
        current_round,
    };
}

void GameState::Save() const {
    if (this->m_Money == 0 || this->m_CurrentRound >= this->m_Rounds.size()) {
        if (std::filesystem::exists(g_SaveFilePath)) std::filesystem::remove(g_SaveFilePath);

        return;
    }

    std::filesystem::create_directories(g_DataDirectory);

    std::fstream file{g_SaveFilePath, std::fstream::binary | std::fstream::out | std::fstream::trunc};
    if (!file) return;

    std::size_t questions_per_round = 0;
    if (!this->m_Rounds.empty()) {
        questions_per_round = this->m_Rounds.front().size();
    }

    file << this->m_Money << ' '
         << this->m_CurrentRound << ' '
         << this->m_Rounds.size() << ' '
         << questions_per_round << '\n';

    for (const auto &round : this->m_Rounds) {
        for (const auto &[category, question] : round) {
            file << question.answers.size() << ' '
                 << static_cast<std::uint16_t>(category) << ' '
                 << question.correct_answer << '\n';

            file << question.question << '\n';

            for (const auto &answer : question.answers) {
                file << answer << '\n';
            }
        }
    }
}

void GameState::NextRound() {
    this->m_CurrentRound++;
}

const std::vector<CategorisedQuestion>* GameState::GetCurrentRoundQuestions() const {
    if (this->m_CurrentRound < 0 || this->m_CurrentRound >= this->m_Rounds.size()) {
        return nullptr;
    }

    return &this->m_Rounds[this->m_CurrentRound];
}
