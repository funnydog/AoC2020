#!/usr/bin/env python3

import sys

def find_id(letters):
    value = 0
    for x in letters:
        if x == "B" or x == "R":
            value = value << 1 | 1
        elif x == "F" or x == "L":
            value <<= 1
        else:
            pass

    return value

# NOTE: unit tests
assert find_id("FBFBBFFRLR") == 357
assert find_id("BFFFBBFRRR") == 567
assert find_id("FFFBBBFRRR") == 119
assert find_id("BBFFBBFRLL") == 820

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            seats = [find_id(line) for line in f]
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    seats.sort(reverse = True)
    highest, expected = None, None
    if seats:
        highest = seats[0]
        expected = highest - 1
        for x in seats[1:]:
            if x != expected:
                break
            expected = x - 1

    print("Part1:", highest)
    print("Part2:", expected)
