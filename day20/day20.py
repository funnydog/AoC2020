#!/usr/bin/env python3

import re, sys, math

UP = 0
RIGHT = 1
DOWN = 2
LEFT = 3
DIR = [(0, -1),(1, 0),(0, 1),(-1, 0)]

class Image(object):
    def __init__(self, rows):
        self.rows = rows
        self.height = len(rows)
        self.width = len(rows[0])

    def rotate(self):
        rows = [[0] * self.height for x in range(self.width)]
        for y, row in enumerate(self.rows):
            for x, val in enumerate(row):
                rows[self.width - 1 - x][y] = val

        self.rows = rows
        self.width, self.height = self.height, self.width

    def flip(self):
        for row in self.rows:
            row.reverse()

    def __getitem__(self, key):
        x, y = key
        return self.rows[y][x]

    def __setitem__(self, key, value):
        x, y = key
        self.rows[y][x] = value

    def count_pixels(self, pixel):
        return sum(row.count(pixel) for row in self.rows)

class Tile(Image):
    def __init__(self, tid, rows):
        super(Tile, self).__init__(rows)
        self.tid = tid
        self.visited = False
        self.px = 0
        self.py = 0
        self.adj = []
        self.borders = []
        assert len(self.rows) == len(self.rows[0])
        assert len(self.rows) == 10

    def compute_borders(self):
        b = []
        b.append("".join(self.rows[0][i] for i in range(10))) # UP
        b.append("".join(self.rows[i][9] for i in range(10))) # RIGHT
        b.append("".join(self.rows[9][i] for i in range(10))) # DOWN
        b.append("".join(self.rows[i][0] for i in range(10))) # LEFT
        b.append("".join(self.rows[0][9-i] for i in range(10))) # UP FLIP
        b.append("".join(self.rows[9-i][9] for i in range(10))) # RIGHT FLIP
        b.append("".join(self.rows[9][9-i] for i in range(10))) # DOWN FLIP
        b.append("".join(self.rows[9-i][0] for i in range(10))) # LEFT FLIP
        self.borders = b

    def rotate(self):
        super(Tile, self).rotate()
        self.compute_borders()

    def flip(self):
        super(Tile, self).flip()
        self.compute_borders()

    def shares_border(self, other):
        for i in borders:
            if i in other.borders:
                return True
        return False

    def align_to(self, other):
        for j in range(8):
            self.rotate()
            if j == 4:
                self.flip()
            for i in range(4):
                if self.borders[(i+2)%4] == other.borders[i]:
                    return i

        return None

def parse_tiles(infile):
    TILEID = re.compile(r"Tile (\d+):")
    tiles = []
    state = 0
    for line in f:
        line = line.strip()
        if state == 0:
            m = TILEID.match(line)
            if m:
                tileid = int(m[1])
                tilerows = []
                state = 1
        else:
            if line:
                tilerows.append(line)
            else:
                tiles.append(Tile(tileid, tilerows))
                state = 0

    if state != 0:
        tiles.append(Tile(tileid, tilerows))

    return tiles

def find_adjacencies(tiles):
    borders = []
    for i in range(len(tiles)):
        # compute the borders for the tile i
        tiles[i].compute_borders()
        borders.append(set(tiles[i].borders))

        # check if the tiles have common borders
        for j in range(i):
            if not borders[i].isdisjoint(borders[j]):
                tiles[i].adj.append(j)
                tiles[j].adj.append(i)

def part1(tiles):
    p = 1
    for t in tiles:
        if len(t.adj) == 2:
            p *= t.tid

    return p

def map_dfs(tiles, i):
    tiles[i].visited = True
    for j in tiles[i].adj:
        b = tiles[j]
        if not b.visited:
            direction = b.align_to(tiles[i])
            dx, dy = DIR[direction]
            b.px = tiles[i].px + dx
            b.py = tiles[i].py + dy
            map_dfs(tiles, j)

def part2(tiles):
    # explore the map starting from an arbitrary tile
    map_dfs(tiles, 0)

    # find the size of the map
    xmin, ymin = 0, 0
    xmax, ymax = 0, 0
    for t in tiles:
        if xmin > t.px:
            xmin = t.px
        if xmax < t.px:
            xmax = t.px
        if ymin > t.py:
            ymin = t.py
        if ymax < t.py:
            ymax = t.py

    width = xmax - xmin + 1
    height = ymax - ymin + 1

    # build the actual image from the tiles
    image = Image([[" " for x in range(8*width)] for y in range(8*height)])
    for t in tiles:
        sx = (t.px - xmin) * 8
        sy = (t.py - ymin) * 8
        for dy in range(8):
            for dx in range(8):
                image[sx+dx, sy+dy] = t[dx+1, dy+1]

    # create the sea monster image
    m = Image([
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    ])
    for j in range(8):
        m.rotate()
        if j == 4:
            m.flip()
        for y in range(image.height - m.height):
            for x in range(image.width - m.width):
                found = True
                for dy in range(m.height):
                    for dx in range(m.width):
                        if m[dx, dy] == "#" and image[x+dx, y+dy] != "#":
                            found = False
                            break
                    if not found:
                        break

                if found:
                    for dy in range(m.height):
                        for dx in range(m.width):
                            if m[dx, dy] == "#":
                                image[x+dx, y+dy] = "O"

    return image.count_pixels("#")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        exit(1)

    with open(sys.argv[1], "rt") as f:
        tiles = parse_tiles(f)

    find_adjacencies(tiles)
    print("Part1:", part1(tiles))
    print("Part2:", part2(tiles))
