#!/usr/bin/env python3

import sys

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

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            expenses = {int(x) for x in f.read().strip().split()}
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", sum_two(expenses, 2020))
    print("Part2:", sum_three(expenses, 2020))
