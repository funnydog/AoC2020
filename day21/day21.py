#!/usr/bin/env python3

import sys

def parse(txt):
    foods = []
    for row in txt.strip().split("\n"):
        words = row.split(" ")
        ingredients = []
        allergens = []
        state = 0
        for w in words:
            if state == 0:
                if w == "(contains":
                    state = 1
                    continue
                ingredients.append(w)
            else:
                allergens.append(w[:-1])
        foods.append((ingredients, allergens))

    return foods

def find_allergens(foods):
    # find the dangerous ingredients
    allergens = {}
    for ilst, alst in foods:
        for a in alst:
            dangerous = allergens.get(a)
            if dangerous:
                allergens[a] = set(i for i in ilst if i in dangerous)
            else:
                allergens[a] = set(ilst)

    # constraint elimination
    allergens = [(k, v) for k, v in allergens.items()]
    solution = []
    while allergens:
        for i in range(len(allergens)-1, -1, -1):
            key, val = allergens[i]
            if not val:
                allergens.pop(i)
            elif len(val) != 1:
                pass
            else:
                val = list(val)[0]
                solution.append((key, val))
                for _, ingr in allergens:
                    ingr.discard(val)

    # sort by allergen name
    solution.sort()
    return solution

def part1(foods, allergens):
    dangerous = set(v for k, v in allergens)
    count = 0
    for ing_list, _ in foods:
        for i in ing_list:
            if i not in dangerous:
                count += 1

    return count

def part2(allergens):
    return ",".join(x[1] for x in allergens)

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

    foods = parse(txt)
    allergens = find_allergens(foods)
    print("Part1:", part1(foods, allergens))
    print("Part2:", part2(allergens))
