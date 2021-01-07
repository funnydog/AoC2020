#!/usr/bin/env python3

import re
import sys

VALIDATORS_PART1 = (
    ("byr", lambda x: True),
    ("iyr", lambda x: True),
    ("eyr", lambda x: True),
    ("hgt", lambda x: True),
    ("hcl", lambda x: True),
    ("ecl", lambda x: True),
    ("pid", lambda x: True),
)

yr_pat = re.compile(r"^\d{4}$")
hgt_pat = re.compile(r"^(\d+)(in|cm)$")
hcl_pat = re.compile(r"^#[0-9a-f]{6}$")
ecl_pat = re.compile(r"^(amb|blu|brn|gry|grn|hzl|oth)$")
pid_pat = re.compile(r"^\d{9}$")

def hgt_check(x):
    m = hgt_pat.match(x)
    if m is None:
        return False
    elif m[2] == "cm":
        return 150 <= int(m[1]) <= 193
    elif m[2] == "in":
        return 59 <= int(m[1]) <= 76
    else:
        return False

VALIDATORS_PART2 = (
    ("byr", lambda x: yr_pat.match(x) and 1920 <= int(x) <= 2002),
    ("iyr", lambda x: yr_pat.match(x) and 2010 <= int(x) <= 2020),
    ("eyr", lambda x: yr_pat.match(x) and 2020 <= int(x) <= 2030),
    ("hgt", hgt_check),
    ("hcl", lambda x: bool(hcl_pat.match(x))),
    ("ecl", lambda x: bool(ecl_pat.match(x))),
    ("pid", lambda x: bool(pid_pat.match(x))),
)

def is_valid(passport, validators):
    for field, fn in validators:
        value = passport.get(field)
        if not (value and fn(value)):
            return False

    return True

def process_batch(txt, validators):
    count = 0
    passport = {}
    for line in txt.split("\n"):
        line = line.strip()
        if line == "":
            if is_valid(passport, validators):
                count += 1

            passport = {}
        else:
            for token in line.split():
                field, value = token.split(":")
                passport[field] = value

    if is_valid(passport, validators):
        count += 1

    return count

# VALIDATORS_MAP = {x:y for x, y in VALIDATORS_PART2}
# assert VALIDATORS_MAP["byr"]("2002") == True
# assert VALIDATORS_MAP["byr"]("2003") == False
# assert VALIDATORS_MAP["hgt"]("60in") == True
# assert VALIDATORS_MAP["hgt"]("190cm") == True
# assert VALIDATORS_MAP["hgt"]("190in") == False
# assert VALIDATORS_MAP["hgt"]("190") == False
# assert VALIDATORS_MAP["hcl"]("#123abc") == True
# assert VALIDATORS_MAP["hcl"]("#123abz") == False
# assert VALIDATORS_MAP["hcl"]("123abc") == False
# assert VALIDATORS_MAP["ecl"]("brn") == True
# assert VALIDATORS_MAP["ecl"]("wat") == False
# assert VALIDATORS_MAP["pid"]("000000001") == True
# assert VALIDATORS_MAP["pid"]("0123456789") == False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "r") as f:
            txt = f.read()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", process_batch(txt, VALIDATORS_PART1))
    print("Part2:", process_batch(txt, VALIDATORS_PART2))
