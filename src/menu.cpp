#include <iostream>
#include <limits>
#include <print>
#include <string_view>

std::size_t ReadSizeInRange(std::string_view input_label, std::size_t min, std::size_t max) {
    if (!input_label.empty()) {
        std::println("{}", input_label);
    }

    while (true) {
        std::print("> ");

        std::size_t choice;
        std::cin >> choice;

        if (std::cin) {
            if (choice >= min && choice <= max) {
                return choice;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::println("Niepoprawny wybor, sprobuj jeszcze raz.");
    }
}