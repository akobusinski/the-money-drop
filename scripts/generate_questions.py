#!/bin/env python3

from typing import Dict, List, Iterable, TypedDict
from dataclasses import dataclass
from enum import IntEnum
import itertools
import pathlib
import json
import sys

class QuestionCategory(IntEnum):
    HISTORIA = 0
    GEOGRAFIA = 1
    NAUKA = 2
    SPORT = 3
    MUZYKA = 4
    FILM_I_TV = 5
    LITERATURA = 6
    TECHNOLOGIA = 7
    MATEMATYKA = 8
    PRZYRODA = 9
    JEDZENIE_I_KUCHNIA = 10
    KULTURA_I_SZTUKA = 11
    CIEKAWOSTKI = 12
    JEZYKI_I_SLOWA = 13
    POLSKA = 14

    def __str__(self):
        return self._name_

@dataclass
class Question:
    question: str
    correct: str
    incorrect: List[str]

class DefinedQuestion(TypedDict):
    category: int
    question: str
    correct: str
    incorrect: List[str]

@dataclass
class Assertion:
    count_constant: str
    message: str

@dataclass
class ContainerGeneration:
    container_name: str
    max_answers: int
    assertion: Assertion

PREAMBLE = [
    "#pragma once",
    "#include <array>",
    "#include <string_view>",
    "#include \"game_state.hpp\"",
]

GENERATIONS = [
    ContainerGeneration(
        "QUESTIONS_FOUR_ANSWERS",
        4,
        Assertion(
            "k_FourAnswerRoundCount",
            "Not enough questions with at least four answers to start a single game.",
        ),
    ),
    ContainerGeneration(
        "QUESTIONS_THREE_ANSWERS",
        3,
        Assertion(
            "k_ThreeAnswerRoundCount",
            "Not enough questions with at least three answers to start a single game.",
        ),
    ),
    ContainerGeneration(
        "QUESTIONS_TWO_ANSWERS",
        2,
        Assertion(
            "k_TwoAnswerRoundCount",
            "Not enough questions with at least two answers to start a single game.",
        ),
    ),
]

class HeaderGenerator:
    def __init__(self, categories: Dict[QuestionCategory, List[Question]]) -> None:
        self.content = ""
        self.categories = categories

    def generate(self):
        if (len(self.content) == 0):
            self.append(PREAMBLE)

            for generation in GENERATIONS:
                self.generate_questions_array(generation.container_name, generation.max_answers)

            self.generate_assertions()

        return self.content

    def append(self, items: Iterable[str]):
        for item in items:
            self.content += item + '\n'
        
        self.content += '\n'
    
    def generate_questions_array(self, name: str, answer_count: int):
        questions = list(self.questions_with_answer_count(answer_count))

        if len(questions) == 0:
            self.append((
                "constexpr const auto %s = std::array<std::pair<QuestionCategory, StaticQuestion>, 0> {};" % name,
            ))
            return

        self.append(itertools.chain(
            ("constexpr const auto %s = std::array {" % name,),
            (
                self.generate_question_entry(category, question)
                for (category, question) in questions
            ),
            ("};",),
        ))

    def generate_question_entry(self, category: QuestionCategory, question: Question):
        answer_count = len(question.incorrect) + 1

        return "\tstd::pair { QuestionCategory::%s, StaticQuestion { \"%s\", \"%s\", std::array<std::string_view, k_MaxAnswers - 1> { %s }, %d } }," % (
            category,
            question.question,
            question.correct,
            ", ".join(f"\"{answer}\"" for answer in question.incorrect),
            answer_count,
        )

    def questions_with_answer_count(self, answer_count: int):
        answer_count -= 1

        return (
            (category, question)
            for (category, questions) in self.categories.items()
            for question in questions
            if len(question.incorrect) == answer_count
        )

    def generate_assertions(self):
        count_list: List[str] = []
        container_list: List[str] = []

        for generation in GENERATIONS:
            count_list.append(generation.assertion.count_constant)
            container_list.append(f"{generation.container_name}.size()")
            self.append((
                "static_assert(",
                f"\t({' + '.join(count_list)}) * k_CategoriesPerRound",
                f"\t\t<= {' + '.join(container_list)},",
                f"\t\"{generation.assertion.message}\"",
                ");",
            ))

def read_questions(path: pathlib.Path) -> Dict[QuestionCategory, List[Question]]:
    questions: Dict[QuestionCategory, List[Question]] = {}

    with open(path, "r") as f:
        while (line := f.readline().strip()):
            data: DefinedQuestion = json.loads(line)
            category = QuestionCategory(data["category"])

            if category not in questions:
                questions[category] = []
        
            questions[category].append(Question(
                question=data["question"],
                correct=data["correct"],
                incorrect=data["incorrect"],
            ))
    
    return questions
    


def main():
    argv = sys.argv
    if len(argv) < 3:
        print(f"Missing required arguments: {argv[0]} <input> <output>", file=sys.stderr)
        return

    input = pathlib.Path(argv[1])
    output = pathlib.Path(argv[2])
    output.parent.mkdir(parents=True, exist_ok=True)

    questions = read_questions(input)

    header_generator = HeaderGenerator(questions)
    output.write_text(header_generator.generate())
    print(f"Wrote header to {output}")

if __name__ == "__main__":
    main()
