#!/usr/bin/env python3

import sys

class Symbol(object):
    def __init__(self, terminal, value):
        self.terminal = terminal
        self.value = value

class Grammar(object):
    def __init__(self):
        self.productions = {}
        self.patched = False

    def parse(self, txt):
        self.productions.clear()
        rows = []
        state = 0
        for line in txt.strip().split("\n"):
            if state == 0:
                if line.strip() == "":
                    state = 1
                    continue

                pid, rest = line.split(": ")
                pid = int(pid)
                lst = []
                for r in rest.split(" | "):
                    if r[0] != "\"":
                        lst.append([Symbol(False, int(x)) for x in r.split(" ")])
                    else:
                        lst.append([Symbol(True, r[1])])
                    self.productions[pid] = lst
            else:
                rows.append(line)

        return rows

    def count_valid_rows(self, rows, patched):
        self.patched = patched
        return sum(1 for row in rows if self.is_valid(row))

    def is_valid(self, txt):
        self.pos = 0
        self.txt = txt
        return self.match(0) and self.pos == len(self.txt)

    def match_all(self, symbols):
        for s in symbols:
            if s.terminal:
                if self.txt[self.pos] != s.value:
                    return False
                else:
                    self.pos += 1
            elif self.patched and s.value == 8:
                if not self.match(42):
                    return False
            elif self.patched and s.value == 11:
                if not self.match(42):
                    return False
                cnt = 0
                while self.match(42):
                    cnt += 1
                if not self.match(31):
                    return False
                while self.match(31):
                    cnt -= 1
                if cnt < 0:
                    return False
            elif not self.match(s.value):
                return False

        return True

    def match(self, pid):
        if self.pos == len(self.txt):
            return False

        tmp = self.pos
        for rule in self.productions[pid]:
            if self.match_all(rule):
                return True
            # backtrack and start another rule
            self.pos = tmp

        return False

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

    g = Grammar()
    rows = g.parse(txt)
    print("Part1:", g.count_valid_rows(rows, False))
    print("Part2:", g.count_valid_rows(rows, True))
