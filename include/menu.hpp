#pragma once
#include <iostream>
#include <print>
#include <span>
#include <string_view>

template <typename T>
struct MenuItem {
    std::string_view label;
    T value;
};

std::size_t ReadSizeInRange(std::string_view input_label, std::size_t min, std::size_t max);

template <typename T>
T ChooseFromMenu(std::string_view menu_label, const std::span<MenuItem<T>> &items) {
    std::println("{}", menu_label);

    for (std::size_t i = 0; i < items.size(); i++) {
        const auto &value = items[i];

        std::println("{}. {}", i + 1, value.label);
    }

    const auto choice = ReadSizeInRange("", 1, items.size());
    return items[choice - 1].value;
}
