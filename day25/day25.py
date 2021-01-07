#!/usr/bin/env python3

import sys

BASE = 7
MODULUS = 20201227

# find the encryption key for the door
def encryption_key(card, door):
    e, key = 1, BASE
    while key != card and key != door:
        key = key * BASE % MODULUS
        e += 1

    return pow(key == door and card or door, e, MODULUS)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            card, door = map(int, f.read().strip().split("\n"))
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", encryption_key(card, door))
