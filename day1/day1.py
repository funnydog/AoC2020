#!/usr/bin/env python3

with open("input") as f:
    expenses = {int(x) for x in f.read().strip().split()}

def sum_two(expenses, total):
    for x in expenses:
        if (total - x) in expenses:
            return x * (total - x)

    return None

def sum_three(expenses, total):
    for x in expenses:
        for y in expenses:
            if (total - x - y) in expenses:
                return x * y * (total - x - y)

    return None

print("Part1:", sum_two(expenses, 2020))
print("Part2:", sum_three(expenses, 2020))
