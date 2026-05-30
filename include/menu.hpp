#pragma once
#include <concepts>
#include <iostream>
#include <optional>
#include <print>
#include <span>
#include <string>
#include <string_view>

/**
 * @brief An item which can be rendered by `ChooseFromMenu`
 * 
 * @tparam T The stored value
 */
template <typename T>
struct MenuItem {
    std::string label;
    T value;
};

/**
 * @brief Prompts the user to enter a number in a provided range
 * 
 * @tparam T Integer type to use
 * @param input_label String to render when prompting the user
 * @param min Minimum value that the user can enter
 * @param max Maximum value that the user can enter
 * @param default_value Default value to return when the user presses enter, if `std::nullopt` does not allow an empty input.
 * @return T The amount chosen by the user, or default, if applicable
 */
template <std::unsigned_integral T>
T ReadSizeInRange(
    const std::optional<std::string_view> input_label,
    const T min,
    const T max,
    const std::optional<T> default_value = std::nullopt
) {
    if (input_label.has_value()) {
        std::println("{}", input_label.value());
    }

    while (true) {
        std::print("> ");

        std::string input;
        std::getline(std::cin, input);

        if (default_value && input.empty()) {
            return *default_value;
        }

        T choice = 0;
        const auto begin = input.data();
        const auto end = begin + input.size();
        const auto [position, error] = std::from_chars(begin, end, choice);

        if (error == std::errc{} && position == end && choice >= min && choice <= max) {
            return choice;
        }

        std::println("Niepoprawny wybor, sprobuj jeszcze raz.");
    }
}

/**
 * @brief Prompts the user to select an option
 * 
 * @tparam T Type of the value associated with each menu item
 * @param menu_label String to render when prompting the user
 * @param items List of items to let the user choose from
 * @return T Value associated with the selected by the user item
 */
template <typename T>
T ChooseFromMenu(std::string_view menu_label, const std::span<MenuItem<T>> &items) {
    std::println("{}", menu_label);

    for (std::size_t i = 0; i < items.size(); i++) {
        const auto &value = items[i];

        std::println("{}. {}", i + 1, value.label);
    }

    const auto choice = ReadSizeInRange<std::size_t>(std::nullopt, 1, items.size());
    return items[choice - 1].value;
}
