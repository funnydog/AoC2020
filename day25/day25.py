#!/usr/bin/env python3

BASE = 7
MODULUS = 20201227

# find the encryption key for the door
def encryption_key(card, door):
    e, key = 1, BASE
    while key != card and key != door:
        key = key * BASE % MODULUS
        e += 1

    return pow(key == door and card or door, e, MODULUS)

import sys
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1], "rt") as f:
        card, door = map(int, f.read().strip().split("\n"))

    print("Part1:", encryption_key(card, door))
