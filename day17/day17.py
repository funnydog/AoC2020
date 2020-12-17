#!/usr/bin/env python3

class Cube(object):
    def __init__(self, cycles, dim):
        self.values = {}
        self.dim = dim
        self.minrange = [1000] * self.dim
        self.maxrange = [-1000] * self.dim

    def clear(self):
        self.minrange = [1000] * self.dim
        self.maxrange = [-1000] * self.dim

    def count(self):
        return sum(self.values.values())

    def next(self, pos, start, end):
        pos[0] += 1
        for i in range(self.dim-1):
            if pos[i] < end[i]:
                break
            pos[i] = start[i]
            pos[i+1] += 1

        return pos

    def get(self, pos):
        return self.values.get(tuple(pos), 0)

    def set(self, pos, value):
        pos = tuple(pos[i] for i in range(self.dim))
        if value:
            self.values[pos] = 1
            for i in range(self.dim):
                if self.minrange[i] > pos[i]:
                    self.minrange[i] = pos[i]
                if self.maxrange[i] < pos[i]:
                    self.maxrange[i] = pos[i]
        else:
            self.values[pos] = 0

    def print(self):
        start = [x for x in self.minrange]
        end = [x+1 for x in self.maxrange]
        pos = start.copy()
        while pos[-1] < end[-1]:
            if pos[:2] == start[:2]:
                if self.dim == 4:
                    print("z = {}, w = {}".format(pos[2], pos[3]))
                else:
                    print("z = {}".format(pos[2]))
            print(self.get(pos) and "#" or ".", end="")
            if (pos[0] == end[0]-1):
               print()
            pos = self.next(pos, start, end)

    def adjacent(self, pos):
        count = 0
        start = [x-1 for x in pos]
        end = [x+2 for x in pos]
        cur = start.copy()
        while cur[-1] < end[-1]:
            if cur != pos:
                count += self.get(cur)
            cur = self.next(cur, start, end)

        return count

    def cycle(self, src):
        self.clear()
        start = [x-1 for x in src.minrange]
        end = [x+2 for x in src.maxrange]
        pos = start.copy()
        while pos[-1] < end[-1]:
            c = src.adjacent(pos)
            if src.get(pos):
                self.set(pos, 2 <= c <= 3)
            else:
                self.set(pos, c == 3)

            pos = self.next(pos, start, end)

def conwaycube(txt, cycles, dim):
    lines = txt.strip().split("\n")
    cube = Cube(cycles, dim)
    other = Cube(cycles, dim)

    for y, line in enumerate(lines):
        for x, v in enumerate(line):
            if v == "#":
                cube.set((x, y, 0, 0), True)

    for i in range(cycles):
        cube, other = other, cube
        cube.cycle(other)

    return cube.count()

txt = """.#.
..#
###"""
assert conwaycube(txt, 6, 3) == 112
assert conwaycube(txt, 6, 4) == 848

with open("input", "rt") as f:
    txt = f.read()

print("Part1:", conwaycube(txt, 6, 3))
print("Part2:", conwaycube(txt, 6, 4))
