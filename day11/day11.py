#!/usr/bin/env python3

import sys
from copy import deepcopy

def make_seats(txt):
    return [list(row) for row in txt.strip().split("\n")]

def count_stable(seats, nextfn, maxocc=4):
    scur = deepcopy(seats)
    snext = deepcopy(seats)
    while True:
        nextfn(scur, snext)
        scur, snext = snext, scur
        if scur == snext:
            break

    return sum(row.count("#") for row in scur)

DIR = ((0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1))

def next_part1(cur, out):
    height, width = len(cur), len(cur[0])
    for y, row in enumerate(cur):
        for x, val in enumerate(row):
            if val == ".":
                continue

            # count the occupied adjacent places
            count  = 0
            for dx, dy in DIR:
                nx = x + dx
                ny = y + dy
                if 0 <= ny < height and 0 <= nx < width:
                    if cur[ny][nx] == "#":
                        count += 1

            # generate the next state for the element at (y,x)
            if val == "L":
                out[y][x] = (count == 0) and "#" or "L"
            else:
                out[y][x] = (count >= 4) and "L" or "#"

def next_part2(cur, out):
    height, width = len(cur), len(cur[0])
    for y, row in enumerate(cur):
        for x, val in enumerate(row):
            if val == ".":
                continue

            # count the occupied adjacent places
            count = 0
            for dx, dy in DIR:
                nx = x + dx
                ny = y + dy
                while 0 <= ny < height and 0 <= nx < width:
                    if cur[ny][nx] == "#":
                        count += 1
                        break
                    elif cur[ny][nx] == "L":
                        break
                    else:
                        nx += dx
                        ny += dy

            # generate the next state for the element at (y,x)
            if val == "L":
                out[y][x] = (count == 0) and "#" or "L"
            else:
                out[y][x] = (count >= 5) and "L" or "#"

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

    s = make_seats(txt)
    print("Part1:", count_stable(s, next_part1))
    print("Part2:", count_stable(s, next_part2))
