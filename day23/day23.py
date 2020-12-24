#!/usr/bin/env python3

def simulate(v, nxt, moves):
    lowest = min(v)
    highest = max(v)
    cur = 0
    while moves > 0:
        moves -= 1

        # pick up the next 3 values
        start = nxt[v[cur]]
        a = v[start]
        end = nxt[a]
        b = v[end]
        end = nxt[b]
        c = v[end]

        # find the new value
        dst = v[cur]
        while True:
            dst -= 1
            if dst < lowest:
                dst = highest
            if dst != a and dst != b and dst != c:
                break

        # adjust the links
        nxt[v[cur]] = nxt[v[end]]
        nxt[v[end]] = nxt[dst]
        nxt[dst] = start

        cur = nxt[v[cur]]

def part1(pattern):
    v = [int(x) for x in pattern.strip()]
    nxt = [None] + [0] * len(v)
    for i in range(len(v)-1):
        nxt[v[i]] = i + 1

    simulate(v, nxt, 100)

    lst = []
    i = nxt[1]
    while v[i] != 1:
        lst.append(v[i])
        i = nxt[v[i]]

    return "".join(map(str, lst))

def part2(pattern):
    v = [int(x) for x in pattern.strip()]
    for i in range(len(v)+1, 1000000+1):
        v.append(i)
    nxt = [None] + [0] * len(v)
    for i in range(len(v)-1):
        nxt[v[i]] = i + 1

    simulate(v, nxt, 10000000)

    i = nxt[1]
    a = v[i]
    i = nxt[a]
    b = v[i]
    return a * b

import sys
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1], "rt") as f:
        pattern = f.read()

    print("Part1:", part1(pattern))
    print("Part2:", part2(pattern))
