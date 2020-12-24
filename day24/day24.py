#!/usr/bin/env python3

WHITE = 0
BLACK = 1

# Mapping of the hexagonal coordinates
# to the cartesian coordinates
#
#      (0, -1) [NW]  [NE] (1,-1)
#               |   /
#               |  /
# (-1,0) [ W]--[O ]--[ E] (1,0)
#             / |
#            /  |
# (-1,1) [SW]  [SE] (0, 1)
#
DIRECTIONS = [
    ("e", (1, 0)),              # EAST
    ("se", (0, 1)),             # SOUTH-EAST
    ("sw", (-1, 1)),            # SOUTH-WEST
    ("w", (-1, 0)),             # WEST
    ("nw", (0,-1)),             # NORTH-WEST
    ("ne", (1,-1)),             # NORTH-EAST
]

def pose_floor(txt):
    floor = set()
    for line in txt.strip().split("\n"):
        # find the coordinates of the tile
        q, r = 0, 0
        while line:
            for d, (dq, dr) in DIRECTIONS:
                if line.startswith(d):
                    q += dq
                    r += dr
                    line = line[len(d):]
                    break
            else:
                print("unrecognized", line)
                exit(1)

        # flip the tile at (q, r)
        pos = (q,r)
        if pos in floor:
            floor.remove(pos)
        else:
            floor.add(pos)

    return floor

def count_black_tiles(floor):
    return len(floor)

def simulate(floor, days):
    # find the area of interest
    qmin = min(q for q, r in floor) - 1
    qmax = max(q for q, r in floor) + 1
    rmin = min(r for q, r in floor) - 1
    rmax = max(r for q, r in floor) + 1

    current, other = floor, set()
    for i in range(days):
        # NOTE: perform one simulation step
        other.clear()
        nqmin, nqmax = qmax, qmin
        nrmin, nrmax = rmax, rmin
        for q in range(qmin, qmax+1):
            for r in range(rmin, rmax+1):
                pos = (q, r)

                # count the neighbours
                neigh = sum(1 for _, (nq, nr) in DIRECTIONS if (pos[0]+nq, pos[1]+nr) in current)

                # apply the rules to the current tile
                black = pos in current
                if black and (neigh == 0 or neigh > 2):
                    other.discard(pos)
                elif (not black and neigh == 2) or black:
                    other.add(pos)
                    if nqmin > q: nqmin = q
                    if nqmax < q: nqmax = q
                    if nrmin > r: nrmin = r
                    if nrmax < r: nrmax = r

        # swap the old and new floors
        current, other = other, current
        qmin, qmax = nqmin-1, nqmax+1
        rmin, rmax = nrmin-1, nrmax+1

import sys
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        exit(1)

    with open(sys.argv[1], "rt") as f:
        floor = pose_floor(f.read())

    print("Part1:", count_black_tiles(floor))
    simulate(floor, 100)
    print("Part2:", count_black_tiles(floor))
