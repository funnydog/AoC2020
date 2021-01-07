#!/usr/bin/env python3

import sys

def spoken_game(seq, count):
    seq = [int(x) for x in seq.strip().split(",")]

    spoken = [0]*count
    for i, x in enumerate(seq):
        spoken[x] = i + 1

    last = seq[-1]
    for i in range(len(seq), count):
        if spoken[last]:
            next_s = i - spoken[last]
        else:
            next_s = 0

        spoken[last] = i
        last = next_s

    return last

# assert spoken_game("0,3,6", 2020) == 436
# assert spoken_game("1,3,2", 2020) == 1
# assert spoken_game("2,1,3", 2020) == 10
# assert spoken_game("1,2,3", 2020) == 27
# assert spoken_game("2,3,1", 2020) == 78
# assert spoken_game("3,2,1", 2020) == 438
# assert spoken_game("3,1,2", 2020) == 1836

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

    print("Part1:", spoken_game(txt, 2020))
    print("Part2:", spoken_game(txt, 30000000))
