#!/usr/bin/env python3

def sum_common_answers(txt):
    sum_any = 0
    sum_all = 0
    answers = [0] * 26
    group_count = 0

    for line in txt.split("\n"):
        line = line.strip()
        if line:
            for letter in line:
                if ord('a') <= ord(letter) <= ord('z'):
                    answers[ord(letter) - ord('a')] += 1
            group_count += 1
        elif group_count:
            for answer in answers:
                if answer > 0:
                    sum_any += 1
                if answer == group_count:
                    sum_all += 1

            answers = [0] * 26
            group_count = 0

    if group_count:
        for answer in answers:
            if answer > 0:
                sum_any += 1
            if answer == group_count:
                sum_all += 1

    return sum_any, sum_all

txt = """abc

a
b
c

ab
ac

a
a
a
a

b"""


assert sum_common_answers(txt) == (11, 6)

with open("input", "rt") as f:
    txt = f.read()

part1, part2 = sum_common_answers(txt)
print("Part1:", part1)
print("Part2:", part2)
