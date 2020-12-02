#!/usr/bin/env python3

import re

pattern = re.compile(r"^(\d+)-(\d+) (\S): (.*)$")

valid1 = 0
valid2 = 0
with open("input", "rt") as f:
    for line in f:
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

print("Part1:", valid1)
print("Part2:", valid2)
