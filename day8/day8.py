#!/usr/bin/env python3

import time

class System(object):
    def __init__(self):
        self.acc = 0
        self.ip = 0
        self.instructions = []

    def load(self, txt):
        self.acc = 0
        self.ip = 0
        self.instructions = []
        for line in txt.strip().split("\n"):
            opcode, value = line.split(" ")
            self.instructions.append([opcode, int(value)])

    def run(self):
        self.acc = 0
        self.ip = 0
        visited = [False] * len(self.instructions)

        while self.ip < len(self.instructions) and not visited[self.ip]:
            visited[self.ip] = True
            op, value = self.instructions[self.ip]
            if op == "acc":
                self.acc += value
                self.ip += 1
            elif op == "jmp":
                self.ip += value
            elif op == "nop":
                self.ip += 1
            else:
                break

        return self.acc

    def analyze(self):
        for ip, instr in enumerate(self.instructions):
            if instr[0] == "nop":
                self.instructions[ip][0] = "jmp"
                self.run()
                self.instructions[ip][0] = "nop"
            elif instr[0] == "jmp":
                self.instructions[ip][0] = "nop"
                self.run()
                self.instructions[ip][0] = "jmp"
            else:
                continue

            if self.ip >= len(self.instructions):
                return self.acc

        return None

txt = """nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6"""

s = System()
s.load(txt)
assert s.run() == 5
assert s.analyze() == 8

with open("input", "rt") as f:
    s.load(f.read())

print("Part1:", s.run())
print("Part2:", s.analyze())
