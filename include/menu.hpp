#include <string_view>
#include <span>
#include <print>
#include <iostream>
#include <unordered_map>

template <typename T>
struct MenuItem {
    std::string_view label;
    T value;
};

template <typename T>
T ChooseFromMenu(std::string_view menu_label, const std::span<MenuItem<T>> &items) {
    std::unordered_map<std::size_t, T> lookup_table;
    std::println("{}", menu_label);

    for (std::size_t i = 0; i < items.size(); i++) {
        auto value = items[i];

        std::println("{}. {}", i + 1, value.label);
        lookup_table.insert({i + 1, value.value});
    }
    
    while (true) {
        std::print("> ");
        size_t choice;
        std::cin >> choice;

        if (std::cin) {
            auto search = lookup_table.find(choice);
            if (search != lookup_table.end()) {
                return search->second;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::println("Niepoprawny wybor, sprobuj jeszcze raz.");
    }
}