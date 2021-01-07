#!/usr/bin/env python3

import re
import sys

def find_valid(txt):
    pattern = re.compile(r"^(\d+)-(\d+) (\S): (.*)$")
    valid1 = 0
    valid2 = 0
    for line in txt.splitlines():
        m = pattern.match(line)
        if m:
            lowest = int(m.group(1))
            highest = int(m.group(2))
            letter = m.group(3)
            password = m.group(4)

            count = 0
            for l in password:
                if l == letter:
                    count += 1

            if lowest <= count <= highest:
                valid1 += 1

            # logical xor
            if (password[lowest-1] == letter) != (password[highest-1] == letter):
                valid2 += 1

    return valid1, valid2

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    part1, part2 = find_valid(txt)
    print("Part1:", part1)
    print("Part2:", part2)
