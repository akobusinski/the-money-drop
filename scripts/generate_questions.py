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
            self.generate_question_categories_arrays()
            self.generate_question_arrays()
            self.generate_random_category_function()
            self.generate_get_questions_function()

        return self.content

    def append(self, items: Iterable[str]):
        for item in items:
            self.content += item + '\n'
        
        self.content += '\n'

    def generate_preamble(self):
        self.append((
            "#pragma once",
            "#include <map>",
            "#include <array>",
            "#include <random>",
            "#include \"question.hpp\"",
        ))
    
    def generate_question_categories_arrays(self):
        self.append(itertools.chain(
            ("constexpr auto CATEGORIES = std::array {",),
            (f"\tQuestionCategory::{category}," for category in self.categories.keys()),
            ("};",),
        ))
    
    def generate_question_arrays(self):
        for (category, questions) in self.categories.items():
            self.append(itertools.chain(
                ("constexpr auto %s_QUESTIONS = std::array {" % str(category),),
                ("\tQuestion { \"%s\", { %s }, %d }," % (
                    question.question,
                    ", ".join(f"\"{i}\"" for i in question.answers),
                    question.correct
                ) for question in questions),
                ("};",)
            ))
    
    def generate_random_category_function(self):
        self.append((
            "QuestionCategory random_category(std::mt19937 &rng) {",
            "\tstatic std::uniform_int_distribution<std::size_t> dist(0, CATEGORIES.size() - 1);",
            "\treturn CATEGORIES[dist(rng)];",
            "}",
        ))
    
    def generate_get_questions_function(self):
        signature = (
            "template <QuestionCategory Category>",
            "constexpr const auto get_questions() {",
        )

        body = "\t"
        for key in self.categories.keys():
            body += '\n'.join((
                 "if constexpr (Category == QuestionCategory::%s) {" % str(key),
                f"\t\treturn {key}_QUESTIONS;",
                 "\t} else ",
            ))
        body += '\n'.join((
            "{",
            "\t\tstatic_assert({}, \"No questions for category found.\")".format(" || ".join(f"Category == QuestionCategory::{i}" for i in self.categories.keys())),
            "\t}"
        ))

        self.append(itertools.chain(
            signature,
            (body, "}",),
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