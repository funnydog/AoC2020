#!/usr/bin/env python3

import sys

def parse(txt):
    p1 = []
    p2 = []
    cur = p1
    for line in txt.strip().split("\n"):
        line = line.strip()
        if line == "":
            cur = p2
        elif line[:6] == "Player":
            pass
        else:
            cur.append(int(line))
    return p1, p2

def wins_p1(p1, p2, recursive):
    a = p1[0]
    b = p2[0]
    if not recursive:
        return a > b
    elif a >= len(p1) or b >= len(p2):
        return a > b
    else:
        return play_game(p1[1:1+a], p2[1:1+b], recursive)

def play_game(p1, p2, recursive):
    history = set()
    while p1 and p2:
        if recursive:
            if tuple(p1) in history:
                return True
            history.add(tuple(p1))

        if wins_p1(p1, p2, recursive):
            p1.append(p1.pop(0))
            p1.append(p2.pop(0))
        else:
            p2.append(p2.pop(0))
            p2.append(p1.pop(0))

    return p1 and True or False

def score(txt, recursive):
    p1, p2 = parse(txt)
    play_game(p1, p2, recursive)
    winner = p1 or p2
    return sum((i+1)*d for i, d in enumerate(reversed(winner)))

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

    print("Part1:", score(txt, False))
    print("Part2:", score(txt, True))
