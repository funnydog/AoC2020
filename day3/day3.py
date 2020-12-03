#!/usr/bin/env python3

from functools import reduce
import operator

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

text_map = """..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#"""

mymap = make_map(text_map)
assert(count_trees(mymap, (3,1)) == 7)

slopes = [(1,1), (3,1), (5,1), (7,1), (1,2)]
assert(336 == mul_slopes(mymap, slopes))

with open("input", "rt") as f:
    mymap = make_map(f.read())

print("Part1:", count_trees(mymap, (3, 1)))
print("Part2:", mul_slopes(mymap, slopes))
