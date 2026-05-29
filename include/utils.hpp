#pragma once

// https://en.cppreference.com/cpp/utility/variant/visit2#Example
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };