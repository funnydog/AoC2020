#!/usr/bin/env python3

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


txt = """939
7,13,x,x,59,x,31,19"""
estimate, buses = txt.strip().split("\n")
assert wait_time(estimate, buses) == 295

def bezout(a, b):
    old_r, r = b, a
    old_s, s = 1, 0

    while r:
        q, rem = divmod(old_r, r)
        old_r, r = r, rem
        old_s, s = s, old_s - q * s

    if old_s < 0:
        old_s += a

    return old_s * b - 1

# naive solution used when I didn't know the extended euclidean gcd
def find_t(a, b):
    if a > b:
        cur = a + 1
        while True:
            if cur % b == 0:
                break
            cur += a
        return cur-1
    else:
        cur = b-1
        while True:
            if cur % a == 0:
                break
            cur += b
        return cur

def find_start(seq):
    seq = [(x == "x") and "x" or int(x) for x in seq.split(",")]
    limit = 1
    for x in seq:
        if x != "x":
            limit *= x
    last = seq[0]
    cur = 0
    offset = 1
    for x in seq[1:]:
        if x == "x":
            offset += 1
        else:
            cur += bezout(last, limit//last) * offset
            #cur += find_t(last, limit // last) * offset
            cur %= limit
            last *= x
            offset = 1
    return cur

assert find_start("7,13,x,x,59,x,31,19") == 1068781
assert find_start("17,x,13,19") == 3417
assert find_start("67,7,59,61") == 754018
assert find_start("67,x,7,59,61") == 779210
assert find_start("67,7,x,59,61") == 1261476
assert find_start("1789,37,47,1889") == 1202161486

with open("input", "rt") as f:
    estimate, buses = f.read().strip().split("\n")

print("Part1:", wait_time(estimate, buses))
print("Part2:", find_start(buses))
