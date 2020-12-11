#!/usr/bin/env python3

from copy import deepcopy

def make_seats(txt):
    return [list(row) for row in txt.strip().split("\n")]

def count_stable(seats, countfn, maxocc=4):
    scur = deepcopy(seats)
    snext = deepcopy(seats)
    height, width = len(seats), len(seats[0])
    while True:
        for y in range(height):
            for x in range(width):
                val = scur[y][x]
                if val == ".":
                    pass
                elif val == "L":
                    if countfn(scur, width, height, x, y) == 0:
                        snext[y][x] = "#"
                    else:
                        snext[y][x] = "L"
                else:
                    if countfn(scur, width, height, x, y) >= maxocc:
                        snext[y][x] = "L"
                    else:
                        snext[y][x] = "#"

        scur, snext = snext, scur
        if scur == snext:
            break

    return sum(row.count("#") for row in scur)

DIR = ((0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1))

def count_part1(seats, width, height, x, y):
    count  = 0
    for dx, dy in DIR:
        nx = x + dx
        ny = y + dy
        if 0 <= ny < height and 0 <= nx < width:
            if seats[ny][nx] == "#":
                count += 1

    return count

def count_part2(seats, width, height, x, y):
    count = 0
    for dx, dy in DIR:
        nx = x + dx
        ny = y + dy
        while 0 <= ny < height and 0 <= nx < width:
            if seats[ny][nx] == "#":
                count += 1
                break
            elif seats[ny][nx] == "L":
                break
            else:
                nx += dx
                ny += dy

    return count

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
assert count_stable(s, count_part1, 4) == 37
assert count_stable(s, count_part2, 5) == 26

with open("input", "rt") as f:
    s = make_seats(f.read())

print("Part1:", count_stable(s, count_part1, 4))
print("Part2:", count_stable(s, count_part2, 5))
