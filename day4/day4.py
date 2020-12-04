#!/usr/bin/env python3

import re

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

# NOTE: unit tests
txt = """ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm

iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
hcl:#cfa07d byr:1929

hcl:#ae17e1 iyr:2013
eyr:2024
ecl:brn pid:760753108 byr:1931
hgt:179cm

hcl:#cfa07d eyr:2025 pid:166559648
iyr:2011 ecl:brn hgt:59in"""
assert process_batch(txt, VALIDATORS_PART1) == 2

VALIDATORS_MAP = {x:y for x, y in VALIDATORS_PART2}
assert VALIDATORS_MAP["byr"]("2002") == True
assert VALIDATORS_MAP["byr"]("2003") == False
assert VALIDATORS_MAP["hgt"]("60in") == True
assert VALIDATORS_MAP["hgt"]("190cm") == True
assert VALIDATORS_MAP["hgt"]("190in") == False
assert VALIDATORS_MAP["hgt"]("190") == False
assert VALIDATORS_MAP["hcl"]("#123abc") == True
assert VALIDATORS_MAP["hcl"]("#123abz") == False
assert VALIDATORS_MAP["hcl"]("123abc") == False
assert VALIDATORS_MAP["ecl"]("brn") == True
assert VALIDATORS_MAP["ecl"]("wat") == False
assert VALIDATORS_MAP["pid"]("000000001") == True
assert VALIDATORS_MAP["pid"]("0123456789") == False

txt = """eyr:1972 cid:100
hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926

iyr:2019
hcl:#602927 eyr:1967 hgt:170cm
ecl:grn pid:012533040 byr:1946

hcl:dab227 iyr:2012
ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277

hgt:59cm ecl:zzz
eyr:2038 hcl:74454a iyr:2023
pid:3556412378 byr:2007"""
assert process_batch(txt, VALIDATORS_PART2) == 0

txt = """pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980
hcl:#623a2f

eyr:2029 ecl:blu cid:129 byr:1989
iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm

hcl:#888785
hgt:164cm byr:2001 iyr:2015 cid:88
pid:545766238 ecl:hzl
eyr:2022

iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719"""
assert process_batch(txt, VALIDATORS_PART2) == 4

# NOTE: actual computing
with open("input", "r") as f:
    txt = f.read()

print("Part1:", process_batch(txt, VALIDATORS_PART1))
print("Part2:", process_batch(txt, VALIDATORS_PART2))
