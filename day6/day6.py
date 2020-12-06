#!/usr/bin/env python3

def sum_common_answers(txt, intersect = False):
    value = 0
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
                if (intersect and answer == group_count) or (not intersect and answer != 0):
                    value += 1

            answers = [0] * 26
            group_count = 0

    if group_count:
        for answer in answers:
            if (intersect and answer == group_count) or (not intersect and answer != 0):
                value += 1

    return value

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

assert sum_common_answers(txt, False) == 11
assert sum_common_answers(txt, True) == 6

with open("input", "rt") as f:
    txt = f.read()

print("Part1:", sum_common_answers(txt, False))
print("Part2:", sum_common_answers(txt, True))
