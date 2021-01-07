#!/usr/bin/env python3

from collections import deque
import sys

def part1(nums):
    d1, d3 = 1, 1
    for i, value in enumerate(nums[1:], 1):
        if value - 1 == nums[i-1]:
            d1 += 1
        else:
            for j in range(1, 4):
                if i - j > 0 and value - 3 == nums[i-j]:
                    d3 += 1
                    break
    return d1 * d3

def part2(nums):
    q = deque([0] * 3)
    count = 1
    i = len(nums)-1
    while i > 0:
        for j in range(3):
            if i > j and nums[i] - nums[i-j-1] <= 3:
                q[j] += count

        count = q.popleft()
        q.append(0)
        i -= 1

    return count

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

    nums = sorted(map(int, txt.splitlines()))

    print("Part1:", part1(nums))

    nums = [0] + nums + [nums[-1]+3]
    print("Part2:", part2(nums))
