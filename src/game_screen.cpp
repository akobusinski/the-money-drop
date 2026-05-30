#include <cstdint>
#include <format>
#include <print>
#include <span>
#include <utility>
#include <variant>
#include <vector>
#include "menu.hpp"
#include "screen.hpp"
#include "utils.hpp"

struct EditAnswer {
    std::size_t answer_index;
};
struct Ready {};
struct NeedsToBeEmpty {};
struct NoMoney {};
using BetAction = std::variant<EditAnswer, Ready, NeedsToBeEmpty, NoMoney>;

/**
 * @brief Creates menu options for a list of categorised questions
 *
 * @param categories Collection of categories paired with their questions
 * @return Menu items that can be passed to `ChooseFromMenu`
 */
std::vector<MenuItem<const OwnedQuestion*>> create_category_options(const std::vector<CategorisedQuestion> &categories) {
    std::vector<MenuItem<const OwnedQuestion*>> options;
    options.reserve(categories.size());

    for (const auto &[category, question] : categories) {
        options.emplace_back(
            std::string { CategoryName(category) },
            &question
        );
    }

    return options;
}

// used once, I reckon the compiler would inline it, but I would rather force it than gamble
/**
 * @brief Counts the amount of answers that the player has bet on
 *
 * @param bets The bets that the player has placed
 * @return The amount of answers that the player has not bet on
 */
inline std::size_t count_empty_answers(const std::vector<std::uint32_t> &bets) {
    std::size_t count = 0;

    for (const auto bet : bets) {
        if (bet == 0) count++;
    }

    return count;
}

/**
 * @brief Creates menu options for editing the player's bets
 *
 * @param question The current question that the player is answering
 * @param bets The bets that the player has placed
 * @param money The amount of money that the player has available to bet
 * @return Menu items that can be passed to `ChooseFromMenu`
 */
std::vector<MenuItem<BetAction>> create_bet_options(
    const OwnedQuestion &question,
    std::vector<std::uint32_t> &bets,
    const std::uint32_t money
) {
    const auto empty_answers = count_empty_answers(bets);
    const bool can_confirm_bets = money == 0 && empty_answers > 0;

    std::vector<MenuItem<BetAction>> options;
    options.reserve(question.answers.size() + (can_confirm_bets ? 1 : 0));

    for (std::size_t index = 0; index < question.answers.size(); index++) {
        const auto fmt = std::format("{}: {} PLN", question.answers[index], bets[index]);
        BetAction action = EditAnswer { index };

        if (bets[index] == 0) {
            if (empty_answers == 1) {
                action = NeedsToBeEmpty {};
            } else if (money == 0) {
                action = NoMoney {};
            }
        }

        options.emplace_back(fmt, action);
    }

    if (can_confirm_bets) {
        options.emplace_back("Jestem gotowy", Ready {});
    }

    return options;
}

/**
 * @brief Lets the player distribute their money across multiple answers
 *
 * @param question The question that the player is betting on
 * @param money The amount of money that the player currently has
 * @return The amount of money placed on the correct answer
 */
std::uint32_t read_bets(const OwnedQuestion &question, const std::uint32_t money) {
    std::vector<std::uint32_t> bets(question.answers.size(), 0);
    std::uint32_t remaining_money = money;

    while (true) {
        std::println("Do rozdysponowania: {} PLN", remaining_money);
        auto bet_options = create_bet_options(question, bets, remaining_money);
        const auto action = ChooseFromMenu("Wybierz odpowiedz do edycji:", std::span { bet_options });

        const bool is_done = std::visit(overloaded {
            [&](const Ready) {
                return true;
            },
            [&](const NeedsToBeEmpty) {
                std::println("Przynajmniej jedna odpowiedz musi pozostac pusta.");
                return false;
            },
            [&](const NoMoney) {
                std::println("Nie masz pieniedzy zeby postawic pieniadze na ta odpowiedz, najpierw zdejmij z innej odpowiedzi.");
                return false;
            },
            [&](const EditAnswer &edit) {
                const auto max_bet = bets[edit.answer_index] + remaining_money;
                const auto new_bet = ReadSizeInRange<std::uint32_t>(
                    std::format("Ile pieniedzy chcesz postawic na to pytanie? (max: {}): ", max_bet),
                    0,
                    max_bet,
                    max_bet
                );

                bets[edit.answer_index] = new_bet;
                remaining_money = max_bet - new_bet;
                return false;
            },
        }, action);

        if (is_done) {
            return bets[question.correct_answer];
        }
    }
}

ScreenAction GameScreen::Show() {
    while (this->m_State.Money() > 0) {
        const auto questions = this->m_State.GetCurrentRoundQuestions();
        if (!questions) break;

        std::println();
        std::println("Runda {}, {} PLN", this->m_State.Round() + 1, this->m_State.Money());

        auto category_options = create_category_options(*questions);
        const auto &question = *ChooseFromMenu("Wybierz kategorie pytania:", std::span { category_options }); // what a monster

        std::println();
        std::println("Pytanie: {}", question.question);

        const auto money_won = read_bets(question, this->m_State.Money());

        std::println();
        std::println(
            "Poprawna odpowiedz: numer {} - {}",
            question.correct_answer + 1,
            question.answers[question.correct_answer]
        );

        this->m_State.SetMoney(money_won);
        this->m_State.NextRound();
        this->m_State.Save();

        if (money_won > 0) {
            std::println("Zachowujesz {} PLN.", this->m_State.Money());
        } else {
            std::println("Straciles wszystkie pieniadze.");
        }
    }

    if (this->m_State.Money() > 0) {
        std::println("Koniec pytan!!! Wygrales {} PLN!!!!!", this->m_State.Money());
    }

    return CloseScreen{};
}
