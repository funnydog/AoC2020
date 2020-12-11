#!/usr/bin/env python3

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

txt="""L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL"""
s = make_seats(txt)
assert count_stable(s, next_part1) == 37
assert count_stable(s, next_part2) == 26

with open("input", "rt") as f:
    s = make_seats(f.read())

print("Part1:", count_stable(s, next_part1))
print("Part2:", count_stable(s, next_part2))
