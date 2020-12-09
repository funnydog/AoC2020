#!/usr/bin/env python3

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


numbers = [int(x) for x in """35
20
15
25
47
40
62
55
65
95
102
117
150
182
127
219
299
277
309
576""".strip().split("\n")]

p = Decoder(5)
assert p.find_solution(numbers) == (127, 62)

numbers = []
with open("input", "rt") as f:
    numbers = [int(x) for x in f]

p = Decoder(25)
outlier, key = p.find_solution(numbers)
print("Part1:", outlier)
print("Part2:", key)