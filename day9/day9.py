#!/usr/bin/env python3

import sys

class Decoder(object):
    def __init__(self, limit):
        self.limit = limit
        self.numbers = []
        self.psums = []

    def add(self, new):
        #removal
        if len(self.numbers) == self.limit:
            self.numbers.pop(0)
            self.psums.pop(0)
            for psum in self.psums:
                psum.pop(0)

        # insertion
        self.psums.append([x+new for x in self.numbers])
        self.numbers.append(new)

    def is_valid(self, x):
        if len(self.numbers) < self.limit:
            return True
        for lst in self.psums:
            if x in lst:
                return True
        return False

    def find_solution(self, seq):
        n = None
        for n in seq:
            if not self.is_valid(n):
                break
            self.add(n)

        i, j = 0, 1
        csum = seq[i]+seq[j]
        while i < j and j < len(seq):
            if csum < n:
                j += 1
                csum += seq[j]
            elif csum > n:
                csum -= seq[i]
                i += 1
            else:
                return n, min(seq[i:j+1])+max(seq[i:j+1])

        return n, None

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: {} <length> <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[2], "rt") as f:
            txt = f.read()
    except:
        print("Cannot open {}".format(sys.argv[2]), file=sys.stderr)
        sys.exit(1)

    numbers = [int(x) for x in txt.splitlines()]

    length = int(sys.argv[1])
    p = Decoder(length)
    outlier, key = p.find_solution(numbers)
    print("Part1:", outlier)
    print("Part2:", key)
