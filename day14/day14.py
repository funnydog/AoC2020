#!/usr/bin/env python3

import re
import sys

MASKP = re.compile(r"mask = ([X10]{36})")
MEMP = re.compile(r"mem\[(\d+)\] = (\d+)")

def part1(program):
    mem = {}
    for line in program.strip().split("\n"):
        m = MASKP.match(line)
        if m:
            mask = list(m[1])
            continue

        m = MEMP.match(line)
        if m:
            address = int(m[1])
            source = int(m[2])
            result = 0
            for i, symbol in enumerate(mask):
                if symbol == "X":
                    result |= source & (1<<(35-i))
                elif symbol == "1":
                    result |= 1<<(35-i)
                else:
                    pass

            mem[address] = result

    return sum(mem.values())

def gen(pattern, i, cur, lst):
    if i == len(pattern):
        num = 0
        for digit in cur:
            num |= (digit == "1") and 1 or 0
            num <<= 1
        lst.append(num)
    elif pattern[i] != "X":
        gen(pattern, i+1, cur + pattern[i], lst)
    else:
        gen(pattern, i+1, cur + "0", lst)
        gen(pattern, i+1, cur + "1", lst)

def part2(program):
    mem = {}
    for line in program.strip().split("\n"):
        m = MASKP.match(line)
        if m:
            mask = list(m[1])
            continue

        m = MEMP.match(line)
        if m:
            address = int(m[1])
            result = int(m[2])
            pattern = []
            for i, symbol in enumerate(mask):
                if symbol != "0":
                    pattern.append(symbol)
                elif address & (1<<(35-i)) != 0:
                    pattern.append("1")
                else:
                    pattern.append("0")

            addrlist = []
            gen(pattern, 0, "", addrlist)
            for addr in addrlist:
                mem[addr] = result

    return sum(mem.values())

# txt = """mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
# mem[8] = 11
# mem[7] = 101
# mem[8] = 0"""
# assert part1(txt) == 165

# txt = """mask = 000000000000000000000000000000X1001X
# mem[42] = 100
# mask = 00000000000000000000000000000000X0XX
# mem[26] = 1"""
# assert part2(txt) == 208

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

    print("Part1:", part1(txt))
    print("Part2:", part2(txt))
