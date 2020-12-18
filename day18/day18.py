#!/usr/bin/env python3

# tokens
NUM = 0
ADD = 1
MUL = 2
LPAR = 3
RPAR = 4
EOF = 5

# expression = factor {"+" factor | "*" factor}*
# factor = number | "(" expression ")"
class Grammar1(object):
    def __init__(self):
        self.pos = 0
        self.cur = (EOF, None)
        self.txt = ""

    def eval(self, txt):
        self.pos = 0
        self.txt = txt
        self.scan_next()
        return self.expression()

    def scan_next(self):
        while self.pos < len(self.txt) and self.txt[self.pos] in " \n\t\r":
            self.pos +=1
        if self.pos == len(self.txt):
            self.cur = (EOF, None)
        elif self.txt[self.pos] in "0123456789":
            start = self.pos
            while self.pos < len(self.txt) and self.txt[self.pos] in "0123456789":
                self.pos += 1
            self.cur = (NUM, self.txt[start:self.pos])
        elif self.txt[self.pos] == "+":
            self.cur = (ADD, None)
            self.pos += 1
        elif self.txt[self.pos] == "*":
            self.cur = (MUL, None)
            self.pos += 1
        elif self.txt[self.pos] == "(":
            self.cur = (LPAR, None)
            self.pos += 1
        elif self.txt[self.pos] == ")":
            self.cur = (RPAR, None)
            self.pos += 1
        else:
            print("Error: {} at {}".format(self.txt, self.pos))
            exit(1)

    # factor = number | "(" expression ")"
    def factor(self):
        if self.cur[0] == NUM:
            value = int(self.cur[1])
            self.scan_next()
        elif self.cur[0] == LPAR:
            self.scan_next()
            value = self.expression()
            if self.cur[0] != RPAR:
                print("Error: found {} expected {}".format(self.cur[0], RPAR))
                exit(1)
            self.scan_next()
        else:
            print("Error: found type {}, expected {} or {}".format(self.cur[0], NUM, LPAR))
            exit(1)

        return value

    # expression = factor {"+" factor | "*" factor}*
    def expression(self):
        value = self.factor()
        while True:
            if self.cur[0] == ADD:
                self.scan_next()
                value += self.factor()
            elif self.cur[0] == MUL:
                self.scan_next()
                value *= self.factor()
            else:
                break
        return value

# expression = term {"+" term}*
# term = factor {"*" factor}*
# factor = number | "(" expression ")"
class Grammar2(Grammar1):
    # term = factor {"*" factor}*
    def term(self):
        value = self.factor()
        while self.cur[0] == ADD:
            self.scan_next()
            value += self.factor()
        return value

    # expression = term {"+" term}*
    def expression(self):
        value = self.term()
        while self.cur[0] == MUL:
            self.scan_next()
            value *= self.term()
        return value

g1 = Grammar1()
g2 = Grammar2()

assert g1.eval("1 + 2 * 3 + 4 * 5 + 6") == 71
assert g1.eval("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 437
assert g1.eval("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 12240
assert g1.eval("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 13632
assert g2.eval("1 + 2 * 3 + 4 * 5 + 6") == 231
assert g2.eval("1 + (2 * 3) + (4 * (5 + 6))") == 51
assert g2.eval("2 * 3 + (4 * 5)") == 46
assert g2.eval("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 669060
assert g2.eval("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 23340

part1, part2 = 0, 0
with open("input", "rt") as f:
    for line in f:
        part1 += g1.eval(line)
        part2 += g2.eval(line)

print("Part1:", part1)
print("Part2:", part2)
