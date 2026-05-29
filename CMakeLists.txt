cmake_minimum_required(VERSION 3.10)
project(PostawNaMilion LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(QUESTIONS_HEADER ${CMAKE_CURRENT_BINARY_DIR}/generated/include/questions.hpp)
set(QUESTIONS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/data/questions.jsonl)
set(GENERATE_QUESTIONS_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_questions.py)

set(SOURCES
    src/main.cpp
    src/game.cpp
    src/game_state.cpp
    src/game_screen.cpp
    src/main_screen.cpp
)

set(HEADERS
    include/game.hpp
    include/question.hpp
    ${QUESTIONS_HEADER}
    include/game_state.hpp
    include/screen.hpp
    include/screen_actions.hpp
    include/menu.hpp
    include/utils.hpp
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_custom_command(
    OUTPUT ${QUESTIONS_HEADER}
    COMMAND ${Python3_EXECUTABLE}
            ${GENERATE_QUESTIONS_SCRIPT}
            ${QUESTIONS_SOURCE}
            ${QUESTIONS_HEADER}
    DEPENDS
        ${QUESTIONS_SOURCE}
        ${GENERATE_QUESTIONS_SCRIPT}
    COMMENT "Generating include/questions.hpp"
    VERBATIM
)
add_custom_target(generate_questions DEPENDS
    ${QUESTIONS_HEADER}
    ${GENERATE_QUESTIONS_SCRIPT}
)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS} ${QUESTIONS_HEADER})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/generated/include
)

add_dependencies(${PROJECT_NAME} generate_questions)