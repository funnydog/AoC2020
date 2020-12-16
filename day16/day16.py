#!/usr/bin/env python3

import re

RULES = re.compile(r"(.*): (\d+)-(\d+) or (\d+)-(\d+)")

def parse(txt):
    rules = {}
    ticket = None
    nearby = []
    state = 0
    for line in txt.strip().split("\n"):
        if state == 0:
            if line == "your ticket:":
                state = 1
                continue

            m = RULES.match(line)
            if m:
                rules[m[1]] = (int(m[2]), int(m[3]), int(m[4]), int(m[5]))
        elif state == 1:
            if line == "nearby tickets:":
                state = 2
                continue
            if line:
                ticket = [int(x) for x in line.split(",")]
        else:
            if line:
                nearby.append([int(x) for x in line.split(",")])

    return rules, ticket, nearby

def part1(txt):
    rules, ticket, nearby = parse(txt)
    error = 0
    for t in nearby:
        for value in t:
            if not any(a <= value <= b or c <= value <= d for a,b,c,d in rules.values()):
                error += value
    return error

txt = """class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12"""
assert part1(txt) == 71

def valid(rules, ticket):
    for value in ticket:
        if not any(a <= value <= b or c <= value <= d for a,b,c,d in rules.values()):
            return False

    return True

def discard_invalid(tickets, rules):
    return [x for x in tickets if valid(rules, x)]

def constraints(rules, tickets):
    constraints = []
    for col in range(len(tickets[0])):
        rnames = []
        for name, (a, b, c, d) in rules.items():
            if all(a <= t[col] <= b or c <= t[col] <= d for t in tickets):
                rnames.append(name)
        constraints.append((col, rnames))

    return constraints

def part2(txt):
    rules, ticket, nearby = parse(txt)
    nearby = discard_invalid(nearby, rules)

    result = []
    columns = constraints(rules, nearby)
    #print("\n".join(str(x[0]) + " " + ",".join(x[1]) for x in columns))
    while columns:
        idx = min(range(len(columns)), key = lambda x: len(columns[x][1]))
        el = columns.pop(idx)
        for i, lst in columns:
            try:
                lst.remove(el[1][0])
            except:
                pass
        result.append((el[0], el[1][0]))

    m = 1
    for col, name in result:
        if name.startswith("departure"):
            m *= ticket[col]

    return m

with open("input", "rt") as f:
    txt = f.read()

print("Part1:", part1(txt))
print("Part2:", part2(txt))
