#!/usr/bin/env python3

import sys

def wait_time(estimate, buses):
    estimate = int(estimate)
    wait = estimate
    bestid = None
    for bus in buses.split(","):
        if bus == "x":
            pass
        else:
            bus = int(bus)
            test = bus - 1 - ((estimate + bus - 1) % bus)
            if wait > test:
                wait = test
                bestid = bus

    return wait * bestid

# NOTE: search by sieving (CRT)
def sieve(seq):
    nums = []
    for t, s in enumerate(seq.split(",")):
        if s != "x":
            n = int(s)
            nums.append((n, (n-t)%n))

    nums.sort(key = lambda x: -x[0])
    p, x = 1, 0
    for n, a in nums:
        while x % n != a:
            x += p
        p *= n

    return x

# assert sieve("7,13,x,x,59,x,31,19") == 1068781
# assert sieve("17,x,13,19") == 3417
# assert sieve("67,7,59,61") == 754018
# assert sieve("67,x,7,59,61") == 779210
# assert sieve("67,7,x,59,61") == 1261476
# assert sieve("1789,37,47,1889") == 1202161486

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

    estimate, buses = txt.strip().split("\n")
    print("Part1:", wait_time(estimate, buses))
    print("Part2:", sieve(buses))
