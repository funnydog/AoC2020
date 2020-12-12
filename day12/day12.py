#!/usr/bin/env python3

def xmul(a, b):
    return (a[0]*b[0]-a[1]*b[1], a[0]*b[1]+a[1]*b[0])

def add(a, b):
    return (a[0]+b[0], a[1]+b[1])

def nmul(a, b):
    return (a[0] * b, a[1] * b)

def manhattan(a):
    return abs(a[0]) + abs(a[1])

NORTH = (0,  -1)
SOUTH = (0,  1)
EAST  = (1,  0)
WEST  = (-1, 0)
LEFT  = (0,  -1)
RIGHT = (0,  1)

def navigate_part1(txt):
    curdir = EAST
    curpos = (0, 0)
    for line in txt.strip().split("\n"):
        ins = line[0]
        n = int(line[1:])
        if ins == "N":
            curpos = add(curpos, nmul(NORTH, n))
        elif ins == "S":
            curpos = add(curpos, nmul(SOUTH, n))
        elif ins == "E":
            curpos = add(curpos, nmul(EAST, n))
        elif ins == "W":
            curpos = add(curpos, nmul(WEST, n))
        elif ins == "F":
            curpos = add(curpos, nmul(curdir, n))
        elif ins == "L":
            for j in range(n//90):
                curdir = xmul(curdir, LEFT)
        elif ins == "R":
            for j in range(n//90):
                curdir = xmul(curdir, RIGHT)
        else:
            pass

    return manhattan(curpos)

def navigate_part2(txt):
    waypoint = add(nmul(EAST, 10), nmul(NORTH, 1))
    curpos = (0, 0)
    for line in txt.strip().split("\n"):
        ins = line[0]
        n = int(line[1:])
        if ins == "N":
            waypoint = add(waypoint, nmul(NORTH, n))
        elif ins == "S":
            waypoint = add(waypoint, nmul(SOUTH, n))
        elif ins == "E":
            waypoint = add(waypoint, nmul(EAST, n))
        elif ins == "W":
            waypoint = add(waypoint, nmul(WEST, n))
        elif ins == "F":
            curpos = add(curpos, nmul(waypoint, n))
        elif ins == "L":
            for j in range(n//90):
                waypoint = xmul(waypoint, LEFT)
        elif ins == "R":
            for j in range(n//90):
                waypoint = xmul(waypoint, RIGHT)

    return manhattan(curpos)

txt = """F10
N3
F7
R90
F11
"""
assert navigate_part1(txt) == 25
assert navigate_part2(txt) == 286

with open("input", "rt") as f:
    txt = f.read()

print("Part1:", navigate_part1(txt))
print("Part2:", navigate_part2(txt))
