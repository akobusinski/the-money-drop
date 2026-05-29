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
    answers: List[str]
    correct: int # 0-based index

class DefinedQuestion(TypedDict):
    category: int
    question: str
    answers:  List[str]
    correct:  int # 0-based index

class HeaderGenerator:
    def __init__(self, categories: Dict[QuestionCategory, List[Question]]) -> None:
        self.content = ""
        self.categories = categories

    def generate(self):
        if (len(self.content) == 0):
            self.generate_preamble()
            self.generate_questions_array()
            self.generate_assertion()

        return self.content

    def append(self, items: Iterable[str]):
        for item in items:
            self.content += item + '\n'
        
        self.content += '\n'
    
    def generate_preamble(self):
        self.append((
            "#pragma once",
            "#include <array>",
            "#include <string_view>",
            "#include \"question.hpp\"",
        ))
    
    def generate_questions_array(self):
        self.append(itertools.chain(
            ("constexpr const auto ALL_QUESTIONS = std::array {",),
            (
                "\tstd::pair { QuestionCategory::%s, StaticQuestion { \"%s\", std::array<std::string_view, k_MaxAnswers> { %s }, %d, %d } },"
                % (
                    category,
                    question.question,
                    ", ".join(f"\"{i}\"" for i in question.answers),
                    len(question.answers),
                    question.correct
                )
                for (category, questions) in self.categories.items()
                for question in questions
            ),
            ("};",),
        ))
    
    def generate_assertion(self):
        self.append((
            "static_assert(",
            "\tk_Rounds * k_CategoriesPerRound <= ALL_QUESTIONS.size(),",
            "\t\"Not enough questions to start a single game.\"",
            ");",
        ))

def read_questions(path: pathlib.Path) -> Dict[QuestionCategory, List[Question]]:
    questions: Dict[QuestionCategory, List[Question]] = {}

    with open(path, "r") as f:
        while (line := f.readline()):
            data: DefinedQuestion = json.loads(line)
            category = QuestionCategory(data["category"])

            if category not in questions:
                questions[category] = []
        
            questions[category].append(Question(
                question=data["question"],
                answers=data["answers"],
                correct=data["correct"],
            ))
    
    return questions
    


def main():
    argv = sys.argv
    if len(argv) < 2:
        print(f"Missing required arguments: {argv[0]} <input> <output>", file=sys.stderr)
        return

    input = pathlib.Path(argv[1])
    output = pathlib.Path(argv[2])
    questions = read_questions(input)

    header_generator = HeaderGenerator(questions)
    output.write_text(header_generator.generate())
    print(f"Wrote header to {output}")

if __name__ == "__main__":
    main()
