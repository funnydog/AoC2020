#!/usr/bin/env python3

from functools import reduce
import operator
import sys

def make_map(txt):
    return txt.strip().split("\n")

def count_trees(mymap, slope):
    height = len(mymap)
    width = len(mymap[0])
    x, y = 0, 0
    trees = 0
    while True:
        x = (x + slope[0]) % width
        y += slope[1]
        if y >= height:
            break
        if mymap[y][x] == "#":
            trees += 1

    return trees

def mul_slopes(mymap, slopes):
    return reduce(operator.mul, [count_trees(mymap, slope) for slope in slopes])


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            mymap = make_map(f.read())
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr);
        sys.exit(1)

    print("Part1:", count_trees(mymap, (3, 1)))

    slopes = [(1,1), (3,1), (5,1), (7,1), (1,2)]
    print("Part2:", mul_slopes(mymap, slopes))
